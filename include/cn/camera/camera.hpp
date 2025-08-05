#pragma once

#include <cn/math/matrix.hpp>
#include <cn/math/vector.hpp>

namespace cn
{

class camera
{
public:
    camera();

    void look_at(const math::vector<float, 3>& position,
                 const math::vector<float, 3>& target);

    const math::matrix<float, 4, 4>& view_matrix() const;
    const math::matrix<float, 4, 4>& projection_matrix() const;

private:
    void update_view();

    math::vector<float, 3> m_position{};
    math::vector<float, 3> m_target{};

    math::matrix<float, 4, 4> m_view{};
    math::matrix<float, 4, 4> m_projection{};
};

} // namespace cn
