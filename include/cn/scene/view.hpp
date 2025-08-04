#pragma once

#include <cn/graphics/material.hpp>
#include <cn/graphics/model.hpp>
#include <cn/graphics/shader.hpp>
#include <cn/math/matrix.hpp>
#include <cn/math/vector.hpp>
#include <cstdint>
#include <stdfloat>

namespace cn
{

class view
{
public:
    explicit view(const model& model);

    void update(std::float32_t delta_time);

    void set_position(const math::vector<std::float32_t, 3>& position);
    void translate(const math::vector<std::float32_t, 3>& offset);

    void set_rotation(const math::vector<std::float32_t, 3>& euler_angles);
    void rotate(const math::vector<std::float32_t, 3>& euler_angles);

    void set_scale(const math::vector<std::float32_t, 3>& scale);
    void scale(const math::vector<std::float32_t, 3>& scale_factor);

    [[nodiscard]] const math::matrix<std::float32_t, 4, 4>& transform() const;

    void                          set_material(const material& material);
    [[nodiscard]] const material& get_material() const;

    [[nodiscard]] const model& get_model() const;

private:
    void update_transform();

    const model&                       m_model;
    material                           m_material;
    math::vector<std::float32_t, 3>    m_position{0.0f, 0.0f, 0.0f};
    math::vector<std::float32_t, 3>    m_rotation{0.0f, 0.0f, 0.0f};
    math::vector<std::float32_t, 3>    m_scale{1.0f, 1.0f, 1.0f};
    math::matrix<std::float32_t, 4, 4> m_transform = math::matrix<std::float32_t, 4, 4>::identity();
    bool                               m_dirty_transform{true};
};

} // namespace cn
