#pragma once

#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <vector>

namespace orb::glfw
{
    struct window_t;
}

namespace orb::vk
{
    struct instance_t;
    struct gpu_t;
    struct device_t;
    struct swapchain_t;

    struct imgui_pass_t
    {
        VkRenderPass                 handle {};
        weak<device_t>               device;
        VkDescriptorPool             desc_pool {};
        VkCommandPool                cmd_pool {};
        std::vector<VkSemaphore>     semaphores;
        std::vector<VkFence>         fences;
        std::vector<VkCommandBuffer> cmds;
        std::vector<VmaAllocation>   mems;
        std::vector<VkImage>         images;
        std::vector<VkImageView>     views;
        std::vector<VkFramebuffer>   fbs;
    };

    class imgui_pass_builder_t
    {
    public:
        weak<glfw::window_t> window;
        weak<instance_t>     instance;
        weak<gpu_t>          gpu;
        weak<device_t>       device;
        weak<swapchain_t>    swapchain;
        size_t               fence_count {};
        size_t               semaphore_count {};
        size_t               cmd_count {};
        size_t               framebuffer_count {};

        std::vector<VkDescriptorPoolSize> pool_sizes;

        [[nodiscard]] static auto prepare(
            weak<glfw::window_t>, weak<instance_t>, weak<gpu_t>, weak<device_t>, weak<swapchain_t>)
            -> result<imgui_pass_builder_t>;

        auto desc_pool_size(desc_types::enum_t type, ui32 count) -> imgui_pass_builder_t&;
        auto semaphores(size_t count) -> imgui_pass_builder_t&;
        auto fences(size_t count) -> imgui_pass_builder_t&;
        auto cmds(size_t count) -> imgui_pass_builder_t&;
        auto images(size_t count) -> imgui_pass_builder_t&;
        auto framebuffers(size_t count) -> imgui_pass_builder_t&;

        [[nodiscard]] auto build() -> result<imgui_pass_t>;

    private:
        imgui_pass_builder_t() = default;
    };

    void destroy(imgui_pass_t& pass);
} // namespace orb::vk

/*
see https://github.com/LucLabarriere/VkOrbital/blob/engine_rework/OrbEngine/src/Graphics2/EditorPass.cpp
for details on how to implement

// on draw:
vkWaitForFences(..., fences.size(), fences.data(), ...);
vkResetFences(..., fences.size(), fences.data(), ...);


// Descriptor pool

// Create Descriptor Pool
// The example only requires a single combined image sampler descriptor for the font image and only uses
one descriptor set (for that)
// If you wish to load e.g. additional textures you may need to alter pools sizes.
{
    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
    check_vk_result(err);
}


Create just one command pool for everything for imgui
not sure yet about command buffers

for (uint32_t i = 0; i < wd->ImageCount; i++)
{
    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
    {
        VkCommandPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.flags = 0;
        info.queueFamilyIndex = queue_family;
        err = vkCreateCommandPool(device, &info, allocator, &fd->CommandPool);
        check_vk_result(err);
    }
    {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = fd->CommandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = 1;
        err = vkAllocateCommandBuffers(device, &info, &fd->CommandBuffer);
        check_vk_result(err);
    }
}


Fences and semaphores:
Basically we need to be able to tell the imgui layer to wait for fences/semaphores
imgui_layer_build_t b;
b.wait_semaphore(previous_render_semaphore) // wait for the game to be renderered for example
 .signal_semaphore(sc_semaphore) // tell the swapchain we are ready to show the image
 .wait_fence(dsqd)
 .wait_semaphore(dsqd);

{
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    err = vkCreateFence(device, &info, allocator, &fd->Fence);
    check_vk_result(err);
}

for (uint32_t i = 0; i < wd->SemaphoreCount; i++)
{
    ImGui_ImplVulkanH_FrameSemaphores* fsd = &wd->FrameSemaphores[i];
    {
        VkSemaphoreCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        err = vkCreateSemaphore(device, &info, allocator, &fsd->ImageAcquiredSemaphore);
        check_vk_result(err);
        err = vkCreateSemaphore(device, &info, allocator, &fsd->RenderCompleteSemaphore);
        check_vk_result(err);
    }
}


// Framebuffers

VkFramebufferCreateInfo info = {};
info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
info.renderPass = wd->RenderPass;
info.attachmentCount = 1;
info.pAttachments = attachment;
info.width = wd->Width;
info.height = wd->Height;
info.layers = 1;
for (uint32_t i = 0; i < wd->ImageCount; i++)
{
    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
    attachment[0] = fd->BackbufferView;
    err = vkCreateFramebuffer(device, &info, allocator, &fd->Framebuffer);
    check_vk_result(err);
}
 * */
