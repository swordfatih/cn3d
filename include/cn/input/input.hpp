#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cn/math/vector.hpp>
#include <string>
#include <stdfloat>
#include <unordered_map>

namespace cn
{

namespace key
{

using key_t = sf::Keyboard::Key;
static constexpr key_t a = sf::Keyboard::Key::A;
static constexpr key_t d = sf::Keyboard::Key::D;
static constexpr key_t q = sf::Keyboard::Key::Q;
static constexpr key_t e = sf::Keyboard::Key::E;
static constexpr key_t s = sf::Keyboard::Key::S;
static constexpr key_t w = sf::Keyboard::Key::W;
static constexpr key_t z = sf::Keyboard::Key::Z;
static constexpr key_t space = sf::Keyboard::Key::Space;

} // namespace key

namespace mouse
{

using button_t = sf::Mouse::Button;
static constexpr button_t left = sf::Mouse::Button::Left;
static constexpr button_t right = sf::Mouse::Button::Right;
static constexpr button_t middle = sf::Mouse::Button::Middle;

} // namespace mouse

class input
{
public:
    static void                            map_axis(const std::string& name, key::key_t negative, key::key_t positive);
    static float                           axis(const std::string& name);
    static bool                            pressed(key::key_t key);
    static bool                            released(key::key_t key);
    static bool                            pressed(mouse::button_t button);
    static bool                            released(mouse::button_t button);
    static math::vector<std::float32_t, 2> mouse_delta();

private:
    static std::unordered_map<std::string, std::pair<key::key_t, key::key_t>> m_axes;
    static math::vector<std::float32_t, 2>                                    m_last_mouse_pos;
};

} // namespace cn
