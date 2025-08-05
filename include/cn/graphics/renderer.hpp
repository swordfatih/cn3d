#pragma once

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cn
{

class window;
class model;
class camera;
class shader;

class renderer
{
public:
    explicit renderer(cn::window& window, bgfx::RendererType::Enum type);
    ~renderer();

    void begin_frame(const cn::camera& camera);
    void draw(const cn::model& model, const cn::shader& shader, const glm::mat4& transform);
    void end_frame();

private:
    cn::window& m_window;
};

} // namespace cn
