#include <cn/camera/camera.hpp>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cn
{

camera::camera()
{
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 100.0f);
    std::memcpy(m_projection.data(), glm::value_ptr(proj), sizeof(glm::mat4));
}

void camera::look_at(const math::vector<float, 3>& position, const math::vector<float, 3>& target)
{
    m_position = position;
    m_target = target;
    update_view();
}

void camera::update_view()
{
    glm::vec3 eye{m_position[0], m_position[1], m_position[2]};
    glm::vec3 center{m_target[0], m_target[1], m_target[2]};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    glm::mat4 view = glm::lookAt(eye, center, up);
    std::memcpy(m_view.data(), glm::value_ptr(view), sizeof(glm::mat4));
}

const math::matrix<float, 4, 4>& camera::view_matrix() const
{
    return m_view;
}

const math::matrix<float, 4, 4>& camera::projection_matrix() const
{
    return m_projection;
}

} // namespace cn
