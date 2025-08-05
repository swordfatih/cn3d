#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cn/camera/camera.hpp>
#include <cn/graphics/model.hpp>
#include <cn/graphics/renderer.hpp>
#include <cn/graphics/shader.hpp>
#include <cn/window/window.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cn
{

renderer::renderer(cn::window& window, bgfx::RendererType::Enum type) : m_window(window)
{
    const auto size = m_window.size();

    bgfx::Init init{};
    init.type = type;
    init.platformData.nwh = m_window.handle().getNativeHandle();
    init.resolution.width = size[0];
    init.resolution.height = size[1];
    init.resolution.reset = BGFX_RESET_VSYNC;

    if(!bgfx::init(init))
    {
        throw std::runtime_error("Failed to initialize BGFX");
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
}

renderer::~renderer()
{
    bgfx::shutdown();
}

void renderer::begin_frame(const cn::camera& camera)
{
    const auto size = m_window.size();

    const float* view = reinterpret_cast<const float*>(camera.view_matrix().data());
    const float* proj = reinterpret_cast<const float*>(camera.projection_matrix().data());

    bgfx::setViewRect(0, 0, 0, size[0], size[1]);
    bgfx::setViewTransform(0, view, proj);
    bgfx::touch(0);
}

void renderer::draw(const cn::model& model, const cn::shader& shader, const glm::mat4& transform)
{
    bgfx::setTransform(glm::value_ptr(transform));
    bgfx::setVertexBuffer(0, model.vertex_buffer());
    bgfx::setIndexBuffer(model.index_buffer());

    bgfx::setState(BGFX_STATE_DEFAULT);

    shader.submit(0);
}

void renderer::end_frame()
{
    bgfx::frame();
}

} // namespace cn
