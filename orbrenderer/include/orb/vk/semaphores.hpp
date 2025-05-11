#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
#include <vector>

namespace orb::vk
{
    struct device_t;

    struct semaphores_view_t
    {
        std::vector<VkSemaphore>          handles;
        std::vector<VkPipelineStageFlags> wait_stages;

        semaphores_view_t()  = default;
        ~semaphores_view_t() = default;

        semaphores_view_t(const semaphores_view_t&)                    = delete;
        auto operator=(const semaphores_view_t&) -> semaphores_view_t& = delete;

        semaphores_view_t(semaphores_view_t&& other)                    = default;
        auto operator=(semaphores_view_t&& other) -> semaphores_view_t& = default;

        [[nodiscard]] auto concat(const semaphores_view_t& other) const -> semaphores_view_t
        {
            semaphores_view_t result;
            result.handles.reserve(handles.size() + other.handles.size());
            result.wait_stages.reserve(wait_stages.size() + other.wait_stages.size());

            for (size_t i = 0; i < handles.size(); ++i)
            {
                result.handles.push_back(handles[i]);
                result.wait_stages.push_back(wait_stages[i]);
            }

            for (size_t i = 0; i < other.handles.size(); ++i)
            {
                result.handles.push_back(other.handles[i]);
                result.wait_stages.push_back(other.wait_stages[i]);
            }

            return result;
        }
    };

    struct semaphores_t
    {

        VkDevice                          device = nullptr;
        std::vector<VkSemaphore>          handles;
        std::vector<VkPipelineStageFlags> wait_stages;

        semaphores_t() = default;

        semaphores_t(const semaphores_t&)                    = delete;
        auto operator=(const semaphores_t&) -> semaphores_t& = delete;

        semaphores_t(semaphores_t&& other) noexcept
            : device(other.device),
              handles(std::move(other.handles)),
              wait_stages(std::move(other.wait_stages))
        {
        }

        auto operator=(semaphores_t&& other) noexcept -> semaphores_t&
        {
            device  = other.device;
            handles = std::move(other.handles);
            wait_stages = std::move(other.wait_stages);

            return *this;
        }

        ~semaphores_t()
        {
            destroy();
        }

        void destroy()
        {
            for (auto& semaphore : handles)
            {
                if (!semaphore)
                {
                    continue;
                }

                vkDestroySemaphore(device, semaphore, nullptr);
                semaphore = nullptr;
            }
        }

        [[nodiscard]] auto view() const -> semaphores_view_t
        {
            semaphores_view_t result;

            result.handles.resize(handles.size());
            result.wait_stages.resize(wait_stages.size());

            for (size_t i = 0; i < handles.size(); ++i)
            {
                result.handles[i]     = handles[i];
                result.wait_stages[i] = wait_stages[i];
            }

            return result;
        }

        [[nodiscard]] auto view(size_t offset, size_t count) const -> semaphores_view_t
        {
            semaphores_view_t result;
            result.handles.resize(count);
            result.wait_stages.resize(count);

            for (size_t i = 0; i < count; ++i)
            {
                result.handles[i]     = handles[offset + i];
                result.wait_stages[i] = wait_stages[offset + i];
            }

            return result;
        }
    };

    class semaphores_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device) -> result<semaphores_builder_t>
        {
            semaphores_builder_t d;
            d.m_device = device;
            return d;
        }

        auto count(size_t count) -> semaphores_builder_t&
        {
            m_count = count;
            return *this;
        }

        auto stage(vk::pipeline_stage_flag stage) -> semaphores_builder_t&
        {
            m_wait_stage = stage;
            return *this;
        }

        [[nodiscard]] auto build() -> result<semaphores_t>
        {
            semaphores_t objs;
            objs.device = m_device->handle;

            objs.handles.resize(m_count);
            objs.wait_stages.resize(m_count, vkenum(m_wait_stage));

            auto sem_info = structs::create::semaphore();

            for (auto& sem : objs.handles)
            {
                if (auto res = vkCreateSemaphore(m_device->handle, &sem_info, nullptr, &sem); res != vkres::ok)
                {
                    return error_t { "Could not create semaphore: {}", vkres::get_repr(res) };
                }
            }

            return objs;
        }

    private:
        weak<device_t>          m_device     = nullptr;
        vk::pipeline_stage_flag m_wait_stage = vk::pipeline_stage_flag::top_of_pipe;
        size_t                  m_count      = 0;
    };
} // namespace orb::vk
