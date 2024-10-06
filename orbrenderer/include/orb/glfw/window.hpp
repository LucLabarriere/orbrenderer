#pragma once

namespace orb::glfw
{
    class driver_t;

    class window_t
    {
    public:
        struct fb_dimensions_t
        {
            int w = 0;
            int h = 0;
        };

        [[nodiscard]] auto get_handle() const -> const void* { return m_handle; }
        [[nodiscard]] auto get_handle() -> void* { return m_handle; }

        template <typename T>
        [[nodiscard]] auto get_handle() const -> const T*
        {
            return static_cast<const T*>(m_handle);
        }

        template <typename T>
        [[nodiscard]] auto get_handle() -> T*
        {
            return static_cast<T*>(m_handle);
        }

        [[nodiscard]] auto get_fb_dimensions() const -> fb_dimensions_t;

        void destroy();

    private:
        friend glfw::driver_t;

        void* m_handle = nullptr;
    };
} // namespace orb::glfw
