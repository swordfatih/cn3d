#include <cn/graphics/light.hpp>

namespace cn
{

light light::make_directional(
    const math::vector<std::float32_t, 3>& direction_,
    const cn::color&                       color_,
    std::float32_t                         intensity_)
{
    light l;
    l.type = light_type::directional;
    l.direction = direction_;
    l.color = color_;
    l.intensity = intensity_;
    return l;
}

light light::make_point(
    const math::vector<std::float32_t, 3>& position_,
    const cn::color&                       color_,
    std::float32_t                         intensity_)
{
    light l;
    l.type = light_type::point;
    l.position = position_;
    l.color = color_;
    l.intensity = intensity_;
    return l;
}

} // namespace cn
