#include <cn/graphics/shader.hpp>
#include <cn/scene/view.hpp>

namespace cn
{

view::view(const model& model) : m_model(model), m_material(shader{})
{
}

void view::update(std::float32_t /*delta_time*/)
{
    if(m_dirty_transform)
    {
        update_transform();
    }
}

void view::set_position(const math::vector<std::float32_t, 3>& position)
{
    m_position = position;
    m_dirty_transform = true;
}

void view::translate(const math::vector<std::float32_t, 3>& offset)
{
    m_position += offset;
    m_dirty_transform = true;
}

void view::set_rotation(const math::vector<std::float32_t, 3>& euler_angles)
{
    m_rotation = euler_angles;
    m_dirty_transform = true;
}

void view::rotate(const math::vector<std::float32_t, 3>& euler_angles)
{
    m_rotation += euler_angles;
    m_dirty_transform = true;
}

void view::set_scale(const math::vector<std::float32_t, 3>& scale)
{
    m_scale = scale;
    m_dirty_transform = true;
}

void view::scale(const math::vector<std::float32_t, 3>& scale_factor)
{
    m_scale *= scale_factor;
    m_dirty_transform = true;
}

void view::update_transform()
{
    m_transform =
        math::matrix<std::float32_t, 4, 4>::translation(m_position) *
        math::matrix<std::float32_t, 4, 4>::rotation_euler(m_rotation) *
        math::matrix<std::float32_t, 4, 4>::scaling(m_scale);

    m_dirty_transform = false;
}

const math::matrix<std::float32_t, 4, 4>& view::transform() const
{
    return m_transform;
}

void view::set_material(const material& material)
{
    m_material = material;
}

const material& view::get_material() const
{
    return m_material;
}

const model& view::get_model() const
{
    return m_model;
}

} // namespace cn
