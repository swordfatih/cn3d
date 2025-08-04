#include <cmath>
#include <cn/camera/camera.hpp>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cn
{

camera::camera()
{
    update_view();
    update_projection();
}

void camera::set_position(const math::vector<std::float64_t, 3>& position)
{
    m_position = position;
    update_view();
}

void camera::look_at(const math::vector<std::float64_t, 3>& target)
{
    glm::dvec3 pos(m_position[0], m_position[1], m_position[2]);
    glm::dvec3 tgt(target[0], target[1], target[2]);
    glm::dvec3 forward = glm::normalize(tgt - pos);

    m_rotation[1] = std::atan2(forward.x, forward.z);
    m_rotation[0] = std::asin(-forward.y);

    update_view();
}

void camera::translate_local(const math::vector<std::float64_t, 3>& offset)
{
    // Build orientation quaternion from Euler angles
    glm::dquat qPitch = glm::angleAxis(static_cast<double>(m_rotation[0]), glm::dvec3(1.0, 0.0, 0.0));
    glm::dquat qYaw = glm::angleAxis(static_cast<double>(m_rotation[1]), glm::dvec3(0.0, 1.0, 0.0));
    glm::dquat qRoll = glm::angleAxis(static_cast<double>(m_rotation[2]), glm::dvec3(0.0, 0.0, 1.0));
    glm::dquat orient = qYaw * qPitch * qRoll;

    glm::dvec3 off(offset[0], offset[1], offset[2]);
    glm::dvec3 worldOff = orient * off;

    m_position[0] += worldOff.x;
    m_position[1] += worldOff.y;
    m_position[2] += worldOff.z;

    update_view();
}

void camera::rotate_view(const math::vector<std::float64_t, 3>& euler_offset)
{
    m_rotation[0] += euler_offset[0];
    m_rotation[1] += euler_offset[1];
    m_rotation[2] += euler_offset[2];
    update_view();
}

const math::matrix<std::float64_t, 4, 4>& camera::view_matrix() const
{
    return m_view;
}

const math::matrix<std::float64_t, 4, 4>& camera::projection_matrix() const
{
    return m_projection;
}

void camera::update_view()
{
    glm::dquat qPitch = glm::angleAxis(static_cast<double>(m_rotation[0]), glm::dvec3(1.0, 0.0, 0.0));
    glm::dquat qYaw = glm::angleAxis(static_cast<double>(m_rotation[1]), glm::dvec3(0.0, 1.0, 0.0));
    glm::dquat qRoll = glm::angleAxis(static_cast<double>(m_rotation[2]), glm::dvec3(0.0, 0.0, 1.0));
    glm::dquat orient = qYaw * qPitch * qRoll;
    glm::dmat4 rotMat = glm::mat4_cast(orient);

    glm::dvec3 pos(m_position[0], m_position[1], m_position[2]);
    glm::dmat4 transMat = glm::translate(glm::dmat4(1.0), -pos);

    glm::dmat4 view = rotMat * transMat;
    std::memcpy(
        m_view.data(),
        glm::value_ptr(view),
        sizeof(glm::dmat4));
}

void camera::update_projection()
{
    double fovy = glm::radians(m_fov);
    double aspect = static_cast<double>(m_aspect_ratio);
    double near = static_cast<double>(m_near);
    double far = static_cast<double>(m_far);

    glm::dmat4 proj = glm::perspective(fovy, aspect, near, far);
    std::memcpy(
        m_projection.data(),
        glm::value_ptr(proj),
        sizeof(glm::dmat4));
}

void camera::set_aspect_ratio(std::float64_t ratio)
{
    m_aspect_ratio = ratio;
    update_projection();
}

} // namespace cn
