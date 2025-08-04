#include <cn/window/window.hpp>

namespace cn
{

window::window(math::vector<std::uint32_t, 2> size, std::string_view title)
    : m_window(sf::VideoMode({size[0], size[1]}), title),
      m_size(size),
      m_title(title)
{
}

bool window::is_open() const
{
    return m_window.isOpen();
}

void window::poll_events()
{
    while(const std::optional event = m_window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void window::capture_mouse(bool capture)
{
    if(capture)
    {
        m_window.setMouseCursorVisible(false);
        m_window.setMouseCursorGrabbed(true);
    }
    else
    {
        m_window.setMouseCursorVisible(true);
        m_window.setMouseCursorGrabbed(false);
    }
}

math::vector<std::uint32_t, 2> window::size() const
{
    return m_size;
}

std::string_view window::title() const
{
    return m_title;
}

sf::Window& window::handle()
{
    return m_window;
}

const sf::Window& window::handle() const
{
    return m_window;
}

math::vector<std::int64_t, 2> window::mouse_position() const
{
    return m_mouse_position;
}

math::vector<std::int64_t, 2> window::mouse_delta() const
{
    return m_mouse_delta;
}

} // namespace cn