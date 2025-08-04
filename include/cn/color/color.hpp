#pragma once

#include <cn/math/vector.hpp>
#include <stdfloat>

namespace cn
{

using color = cn::math::vector<std::float32_t, 4>;

} // namespace cn

namespace cn::colors
{

inline constexpr color white = {1.0f, 1.0f, 1.0f, 1.0f};
inline constexpr color black = {0.0f, 0.0f, 0.0f, 1.0f};
inline constexpr color gray = {0.5f, 0.5f, 0.5f, 1.0f};
inline constexpr color red = {1.0f, 0.0f, 0.0f, 1.0f};
inline constexpr color green = {0.0f, 1.0f, 0.0f, 1.0f};
inline constexpr color blue = {0.0f, 0.0f, 1.0f, 1.0f};
inline constexpr color yellow = {1.0f, 1.0f, 0.0f, 1.0f};
inline constexpr color magenta = {1.0f, 0.0f, 1.0f, 1.0f};
inline constexpr color cyan = {0.0f, 1.0f, 1.0f, 1.0f};
inline constexpr color transparent = {0.0f, 0.0f, 0.0f, 0.0f};
inline constexpr color warm_white = {1.0f, 0.94f, 0.88f, 1.0f};
inline constexpr color cool_white = {0.88f, 0.96f, 1.0f, 1.0f};

} // namespace cn::colors