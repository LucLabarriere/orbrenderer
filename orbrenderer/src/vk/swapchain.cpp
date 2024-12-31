#include "orb/vk/swapchain.hpp"

#include "orb/glfw/driver.hpp"
#include "orb/glfw/window.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/instance.hpp"

#include "imgui_impl_vulkan.h"

#include <orb/eval.hpp>
#include <orb/flux.hpp>
#include <orb/utility.hpp>

namespace orb::vk
{
    auto swapchain_builder_t::prepare(weak<instance_t>     instance,
                                      weak<gpu_t>          gpu,
                                      weak<device_t>       device,
                                      weak<glfw::window_t> window) -> result<swapchain_builder_t>
    {
        return swapchain_builder_t {
            .window   = window,
            .instance = instance,
            .gpu      = gpu,
            .device   = device,
        };
    }

    auto swapchain_builder_t::fb_dimensions(i32 w, i32 h) -> swapchain_builder_t&
    {
        sc.width  = (ui32)w;
        sc.height = (ui32)h;
        return *this;
    }

    auto swapchain_builder_t::fb_dimensions_from_window() -> swapchain_builder_t&
    {
        auto dims = window->get_fb_dimensions();
        sc.width  = (ui32)dims.w;
        sc.height = (ui32)dims.h;
        return *this;
    }

    auto swapchain_builder_t::present_queue_family_index(ui32 index) -> swapchain_builder_t&
    {
        present_qf_index = index;
        return *this;
    }

    auto swapchain_builder_t::build() -> result<swapchain_t>
    {
        auto surface_res = glfw::driver_t::create_vk_surface(instance->handle, *window);
        if (!surface_res) return surface_res.error();
        auto surface = surface_res.value();

        sc.window   = window;
        sc.device   = device;
        sc.gpu      = gpu;
        sc.instance = instance->handle;
        sc.surface  = surface;

        // Check for WSI support
        VkBool32 res {};
        vkGetPhysicalDeviceSurfaceSupportKHR(gpu->handle, present_qf_index, surface, &res);
        if (res != VK_TRUE) { return error_t { "Error no WSI support on GPU." }; }

        sc.format.format = orb::eval | [&] {
            ui32                            count {};
            std::vector<VkSurfaceFormatKHR> avail_formats;
            vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->handle, surface, &count, nullptr);
            avail_formats.resize(count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->handle, surface, &count, avail_formats.data());

            if (count == 1)
            {
                if (avail_formats.back().format == formats::undefined) { return formats.front(); }
                else
                {
                    const auto& ret      = avail_formats.back();
                    sc.format.colorSpace = ret.colorSpace;
                    return ret.format;
                }
            }
            else
            {
                for (const auto request : formats)
                {
                    for (const auto avail : avail_formats)
                    {
                        if (request == avail.format && sc.format.colorSpace == avail.colorSpace)
                        {
                            return avail.format;
                        }
                    }
                }

                const auto& ret      = avail_formats.front();
                sc.format.colorSpace = ret.colorSpace;
                return ret.format;
            }
        };

        sc.present_mode = orb::eval | [&] {
            // Request a certain mode and confirm that it is available. If not use VK_PRESENT_MODE_FIFO_KHR
            // which is mandatory
            ui32                          count {};
            std::vector<VkPresentModeKHR> avail_modes;
            vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->handle, surface, &count, nullptr);
            avail_modes.resize(count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->handle, surface, &count, avail_modes.data());
            for (const auto& request : present_modes)
            {
                for (const auto& avail : avail_modes)
                {
                    if (request == avail) { return request; }
                }
            }

            return present_modes::fifo_khr; // Always available
        };

        if (sc.info.minImageCount == 0)
        {
            sc.info.minImageCount = orb::eval | [&] {
                switch (sc.present_mode)
                {
                case vk::present_modes::mailbox_khr: return 3;
                case vk::present_modes::fifo_khr:
                case vk::present_modes::fifo_relaxed_khr: return 2;
                case vk::present_modes::immediate_khr: return 1;

                default:
                    panic("Unknown present mode: {}. Something went wrong during the vulkan setup",
                          (ui32)sc.present_mode);
                }
            };
        }

        sc.info.surface          = surface;
        sc.info.imageFormat      = sc.format.format;
        sc.info.imageColorSpace  = sc.format.colorSpace;
        sc.info.imageArrayLayers = 1;
        sc.info.imageUsage       = image_usage_flags::color_attachment;
        sc.info.imageSharingMode = sharing_modes::exclusive; // Assume that graphics family == present family
        sc.info.preTransform     = surface_transform_flag::identity_khr;
        sc.info.compositeAlpha   = composite_alpha_flag::opaque_khr;
        sc.info.presentMode      = sc.present_mode;
        sc.info.clipped          = VK_TRUE;

        if (auto r = sc.rebuild(); !r) { return r.error(); };

        return sc;
    }

    auto swapchain_t::rebuild() -> result<void>
    {
        info.oldSwapchain = handle;
        auto dims         = window->get_fb_dimensions();
        width             = (ui32)dims.w;
        height            = (ui32)dims.h;

        if (auto res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->handle, surface, &cap);
            res != vkres::ok)
        {
            return error_t { "Could not retrieve surface capabilities: {}", vkres::get_repr(res) };
        }

        info.minImageCount = std::max(info.minImageCount, cap.minImageCount);

        if (cap.maxImageCount != 0)
        {
            info.minImageCount = std::min(info.minImageCount, cap.maxImageCount);
        }

        if (cap.currentExtent.width == 0xffffffff)
        {
            info.imageExtent.width  = width;
            info.imageExtent.height = height;
        }
        else
        {
            width                   = cap.currentExtent.width;
            height                  = cap.currentExtent.height;
            info.imageExtent.width  = cap.currentExtent.width;
            info.imageExtent.height = cap.currentExtent.height;
        }

        extent.width  = width;
        extent.height = height;

        VkSwapchainKHR new_handle = nullptr;

        if (auto r = vkCreateSwapchainKHR(device->handle, &info, nullptr, &new_handle); r != vkres::ok)
        {
            return error_t { "Could not create the swapchain: {}", vkres::get_repr(r) };
        }

        if (handle)
        {
            for (auto& view : views)
            {
                vkDestroyImageView(device->handle, view, nullptr);
            }

            vkDestroySwapchainKHR(device->handle, handle, nullptr);
        }

        handle = new_handle;

        if (auto r = vkGetSwapchainImagesKHR(device->handle, handle, &img_count, nullptr);
            r != vkres::ok)
        {
            return error_t { "Could not retrieve swapchain image count: {}", vkres::get_repr(r) };
        }

        images.resize(img_count);

        if (auto r = vkGetSwapchainImagesKHR(device->handle, handle, &img_count, images.data());
            r != vkres::ok)
        {
            return error_t { "Could not retrieve swapchain images: {}", vkres::get_repr(r) };
        }

        views.resize(img_count);

        auto view_info   = structs::create::image_view();
        view_info.format = format.format;

        VkImageSubresourceRange image_range = { image_aspect_flags::color, 0, 1, 0, 1 };
        view_info.subresourceRange          = image_range;

        for (auto [img, view] : flux::zip_all_mut(images, views))
        {
            view_info.image = img;
            if (auto r = vkCreateImageView(device->handle, &view_info, nullptr, &view); r != vkres::ok)
            {
                return error_t { "Could not create swapchain image view: {}", vkres::get_repr(r) };
            }

            device->set_name(img, "Swapchain image");
            device->set_name(view, "Swapchain image view");
        }

        return {};
    }

    auto acquire_img(swapchain_t& sc, VkSemaphore sem, VkFence fence, ui64 timeout) -> img_res_t
    {
        ui32      frame {};
        img_res_t res;

        const auto r = vkAcquireNextImageKHR(sc.device->handle, sc.handle, timeout, sem, fence, &frame);

        if (r == vkres::ok)
        {
            res.content.emplace<0>(frame);
            return res;
        }

        res.content.emplace<1>(r);
        return res;
    }

    auto present_img(swapchain_t& sc, VkQueue queue, std::span<VkSemaphore> sem, ui32 frame_index)
        -> img_res_t
    {
        static auto info        = vk::structs::present();
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores    = sem.data();
        info.swapchainCount     = sem.size();
        info.pSwapchains        = &sc.handle;
        info.pImageIndices      = &frame_index;

        img_res_t  res;
        const auto r = vkQueuePresentKHR(queue, &info);

        if (r == vkres::ok)
        {
            res.content.emplace<0>();
            return res;
        }

        res.content.emplace<1>(r);
        return res;
    }

    void destroy(swapchain_t& swapchain)
    {
        for (auto& view : swapchain.views)
        {
            vkDestroyImageView(swapchain.device->handle, view, nullptr);
        }

        vkDestroySwapchainKHR(swapchain.device->handle, swapchain.handle, nullptr);
        vkDestroySurfaceKHR(swapchain.instance, swapchain.info.surface, nullptr);
    }

} // namespace orb::vk
