#pragma once

#include <cn/math/matrix.hpp>
#include <cn/math/vector.hpp>
#include <stdfloat>

namespace cn
{

class camera
{
public:
    camera();

    void set_position(const math::vector<std::float64_t, 3>& position);
    void look_at(const math::vector<std::float64_t, 3>& target);
    void translate_local(const math::vector<std::float64_t, 3>& offset);
    void rotate_view(const math::vector<std::float64_t, 3>& euler_offset);
    void set_aspect_ratio(std::float64_t ratio);

    const math::matrix<std::float64_t, 4, 4>& view_matrix() const;
    const math::matrix<std::float64_t, 4, 4>& projection_matrix() const;

private:
    void update_view();
    void update_projection();

    math::vector<std::float64_t, 3> m_position{};
    math::vector<std::float64_t, 3> m_rotation{}; // Euler angles in radians

    math::matrix<std::float64_t, 4, 4> m_view{};
    math::matrix<std::float64_t, 4, 4> m_projection{};

    std::float64_t m_fov = 60.0;
    std::float64_t m_near = 0.1;
    std::float64_t m_far = 100.0;
    std::float64_t m_aspect_ratio = 16.0 / 9.0;
};

} // namespace cn
