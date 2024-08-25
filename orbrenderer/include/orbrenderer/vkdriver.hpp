#pragma once

#include <orb/box.hpp>
#include <orb/result.hpp>

namespace orb::vk
{
    namespace details { struct handles_t; }

    class driver
    {
    public:
        driver(const driver&)                        = delete;
        driver(driver&&) noexcept                    = default;
        auto operator=(const driver&) -> driver&     = delete;
        auto operator=(driver&&) noexcept -> driver& = default;

        ~driver();

        static auto create() -> result<driver>;

        void        loop();
        auto        terminate() -> result<void>;

    private:
        driver(box<details::handles_t>);

    private:
        box<details::handles_t> m_handles = nullptr;
    };
} // namespace orb::vk
