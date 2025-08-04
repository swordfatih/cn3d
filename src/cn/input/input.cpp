#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cn/input/input.hpp>

namespace cn
{

std::unordered_map<std::string, std::pair<key::key_t, key::key_t>> input::m_axes{};
math::vector<std::float32_t, 2>                                    input::m_last_mouse_pos{};

void input::map_axis(const std::string& name, key::key_t negative, key::key_t positive)
{
    m_axes[name] = {negative, positive};
    auto pos = sf::Mouse::getPosition();
    m_last_mouse_pos = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
}

float input::axis(const std::string& name)
{
    auto it = m_axes.find(name);
    if(it == m_axes.end())
    {
        return 0.0f;
    }
    bool neg = sf::Keyboard::isKeyPressed(it->second.first);
    bool pos = sf::Keyboard::isKeyPressed(it->second.second);
    return (pos ? 1.0f : 0.0f) + (neg ? -1.0f : 0.0f);
}

bool input::pressed(key::key_t key)
{
    return sf::Keyboard::isKeyPressed(key);
}

bool input::released(key::key_t key)
{
    return !sf::Keyboard::isKeyPressed(key);
}

bool input::pressed(mouse::button_t button)
{
    return sf::Mouse::isButtonPressed(button);
}

bool input::released(mouse::button_t button)
{
    return !sf::Mouse::isButtonPressed(button);
}

math::vector<std::float32_t, 2> input::mouse_delta()
{
    auto                            pos = sf::Mouse::getPosition();
    math::vector<std::float32_t, 2> current{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    math::vector<std::float32_t, 2> delta{current[0] - m_last_mouse_pos[0],
                                          current[1] - m_last_mouse_pos[1]};
    m_last_mouse_pos = current;
    return delta;
}

} // namespace cn
