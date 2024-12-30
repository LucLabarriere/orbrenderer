#pragma once

#include "orb/vk/attachments.hpp"
#include "orb/vk/subpasses.hpp"
#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

namespace orb::vk
{
    struct render_pass_t
    {
        VkRenderPass          handle = nullptr;
        VkDevice              device = nullptr;
        VkRenderPassBeginInfo begin_info {};
        VkClearValue          clear_color { 0.0f, 0.0f, 0.0f, 1.0f };

        void bind_color() {
            begin_info.pClearValues = &clear_color;
        }
    };

    class render_pass_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice) -> result<render_pass_builder_t>;
        [[nodiscard]] auto        build(subpasses_t&, attachments_t&) -> result<render_pass_t>;

        auto clear_color(VkClearValue&& color) -> render_pass_builder_t&
        {
            m_color = color;
            return *this;
        }

    private:
        VkDevice     m_device = nullptr;
        VkClearValue m_color {};
    };

    void begin(render_pass_t&, VkCommandBuffer&);
    void end(render_pass_t&, VkCommandBuffer&);
    void destroy(render_pass_t&);
} // namespace orb::vk
