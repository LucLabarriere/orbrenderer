#include <array>
#include <functional>
#include <iostream>
#include <vector>

#include <orb/eval.hpp>
#include <orb/flux.hpp>
#include <orb/glfw.hpp>
#include <orb/vk.hpp>

auto main() -> int
{
    using namespace orb;

    try
    {

        // Initialize GLFW and the window
        glfw::driver_t::initialize().throw_if_error();
        glfw::window_t window = glfw::driver_t::create_window_for_vk().unwrap();

        // Initialize the vulkan instance
        vk::instance_t instance = orb::eval | [] {
            constexpr auto portability = vk::khr_extensions::portability_enumeration;

            auto b = vk::instance_builder_t::prepare().unwrap();

            if (b.is_ext_available(portability))
            {
                b.add_extension(portability);
                b.create_info.flags |= vk::instance_create::portability;
            }

            return b.add_glfw_required_extensions()
                .add_extension(vk::khr_extensions::device_properties_2)
                .debug_layer(vk::validation_layers::validation)
                .add_extension(vk::extensions::debug_report)
                .build()
                .unwrap();
        };

        // Select the GPU
        box<vk::gpu_t> gpu = orb::eval | [&] {
            auto gpus = vk::available_gpus_t::create(instance.handle).unwrap();

            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::discrete) { return gpus.select(i); }
            }

            println("No discrete GPU found, trying out integrated GPUs");
            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::integrated) { return gpus.select(i); }
            }

            println("No integrated GPU found either, return first available device");

            return gpus.select(0);
        };

        // Print informations on select gpu
        describe_gpu(*gpu);

        // Select queue family
        vk::queue_family_t graphics_queue_family = orb::eval | [&]() -> vk::queue_family_t& {
            for (auto& qf : gpu->queue_families)
            {
                if (qf.properties.queueFlags & vk::queue_families::graphics) { return qf; }
            }

            panic("No suitable queue family found");
        };

        println("- Selected queue family {} with {} queues",
                graphics_queue_family.index,
                graphics_queue_family.properties.queueCount);

        // Create vulkan device
        constexpr std::array queue_priorities { 1.0f };

        vk::device_t device = //
            vk::device_builder_t::prepare()
                .unwrap()
                .add_extension(vk::khr_extensions::swapchain)
                .add_queues(graphics_queue_family, queue_priorities)
                .build(gpu.getmut())
                .unwrap();

        println("- Created vulkan device");

        // Create swapchain
        vk::swapchain_t swapchain = //
            vk::swapchain_builder_t::prepare(weak { &instance },
                                             gpu.getmut(),
                                             weak { &device },
                                             weak { &window })
                .unwrap()
                .fb_dimensions_from_window()
                .present_queue_family_index(0)

                .color_space(vk::color_spaces::srgb_nonlinear_khr)
                .format(vk::formats::b8g8r8a8_unorm)
                .format(vk::formats::r8g8b8a8_unorm)
                .format(vk::formats::b8g8r8_unorm)
                .format(vk::formats::r8g8b8_unorm)

                .present_mode(vk::present_modes::mailbox_khr)
                .present_mode(vk::present_modes::immediate_khr)
                .present_mode(vk::present_modes::fifo_khr)

                .build()
                .unwrap();

        println("- Created swapchain");

        // Terminate glfw
        window.destroy();
        println("- Destroyed window");

        vk::destroy(swapchain);
        vk::destroy(device);
        vk::destroy(instance);

        glfw::driver_t::terminate();
        println("- Terminated GLFW");
    }
    catch (const orb::exception& e)
    {
        println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
