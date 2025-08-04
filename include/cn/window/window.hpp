#pragma once

#include <SFML/Window.hpp>
#include <cn/math/vector.hpp>
#include <cstdint>
#include <string_view>

namespace cn
{

class window
{
public:
    window(math::vector<std::uint32_t, 2> size, std::string_view title);
    ~window() = default;

    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(window&&) = default;
    window& operator=(window&&) = default;

    [[nodiscard]] bool is_open() const;
    void               poll_events();

    void capture_mouse(bool capture);

    [[nodiscard]] math::vector<std::uint32_t, 2> size() const;
    [[nodiscard]] std::string_view               title() const;

    [[nodiscard]] sf::Window&                   handle();
    [[nodiscard]] const sf::Window&             handle() const;
    [[nodiscard]] math::vector<std::int64_t, 2> mouse_position() const;
    [[nodiscard]] math::vector<std::int64_t, 2> mouse_delta() const;

private:
    sf::Window                     m_window;
    math::vector<std::uint32_t, 2> m_size;
    std::string                    m_title;

    math::vector<std::int64_t, 2> m_mouse_position;
    math::vector<std::int64_t, 2> m_mouse_delta;
};

} // namespace cn