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
        VkRenderPass handle = nullptr;
        VkDevice     device = nullptr;
    };

    class render_pass_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice) -> result<render_pass_builder_t>;
        [[nodiscard]] auto        build(subpasses_t&, attachments_t&) -> result<render_pass_t>;

    private:
        VkDevice m_device = nullptr;
    };

    void destroy(render_pass_t&);
} // namespace orb::vk
