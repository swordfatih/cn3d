#pragma once

#include <cn/color/color.hpp>
#include <cn/math/vector.hpp>
#include <cstdint>
#include <stdfloat>

namespace cn
{

struct light
{
    enum class light_type : uint8_t
    {
        directional,
        point
    };

    light_type                      type;
    math::vector<std::float32_t, 3> direction;
    math::vector<std::float32_t, 3> position;
    cn::color                       color;
    std::float32_t                  intensity;

    static light make_directional(const math::vector<std::float32_t, 3>& direction, const cn::color& color, std::float32_t intensity = 1.0f);
    static light make_point(const math::vector<std::float32_t, 3>& position, const cn::color& color, std::float32_t intensity = 1.0f);
};

} // namespace cn