#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cn/camera/camera.hpp>
#include <cn/graphics/material.hpp>
#include <cn/graphics/model.hpp>
#include <cn/graphics/renderer.hpp>
#include <cn/math/vector.hpp>
#include <cn/scene/scene.hpp>
#include <cn/scene/view.hpp>
#include <cn/window/window.hpp>

namespace cn
{

renderer::renderer(cn::window& window, bgfx::RendererType::Enum type) : m_window(window)
{
    const auto size = window.size();

    bgfx::Init init{};
    init.type = type;
    init.resolution.width = size[0];
    init.resolution.height = size[1];
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.nwh = m_window.handle().getNativeHandle();

    bgfx::init(init);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    u_lightDir = bgfx::createUniform("u_lightDir", bgfx::UniformType::Vec4);
    u_lightColor = bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4);
    u_lightIntensity = bgfx::createUniform("u_lightIntensity", bgfx::UniformType::Vec4);
}

renderer::~renderer()
{
    bgfx::destroy(u_lightDir);
    bgfx::destroy(u_lightColor);
    bgfx::destroy(u_lightIntensity);
    bgfx::shutdown();
}

void renderer::begin_frame()
{
    const auto size = m_window.size();

    // Clear view before rendering
    bgfx::setViewClear(m_main_view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(m_main_view, 0, 0, size[0], size[1]);
    bgfx::touch(m_main_view);
}

void renderer::draw_depth_pass(const cn::scene& scene, const cn::camera& camera)
{
    const auto   size = m_window.size();
    const float* view = reinterpret_cast<const float*>(camera.view_matrix().data());
    const float* proj = reinterpret_cast<const float*>(camera.projection_matrix().data());

    bgfx::setViewRect(m_depth_view, 0, 0, size[0], size[1]);
    bgfx::setViewClear(m_depth_view, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx::setViewTransform(m_depth_view, view, proj);
    bgfx::touch(m_depth_view);

    for(const auto& drawable: scene.views()) // Use views directly
    {
        const float* model = reinterpret_cast<const float*>(drawable->transform().data());
        bgfx::setTransform(model);
        bgfx::setVertexBuffer(0, drawable->get_model().vertex_buffer());
        bgfx::setIndexBuffer(drawable->get_model().index_buffer());
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(m_depth_view, drawable->get_material().depth_program());
    }
}

void renderer::render(const cn::scene& scene, const cn::camera& camera)
{
    const auto   size = m_window.size();
    const float* view = reinterpret_cast<const float*>(camera.view_matrix().data());
    const float* proj = reinterpret_cast<const float*>(camera.projection_matrix().data());

    bgfx::setViewRect(m_main_view, 0, 0, size[0], size[1]);
    bgfx::setViewTransform(m_main_view, view, proj);

    // Use first light in scene (for simplicity)
    const cn::light* active_light = nullptr;
    for(const auto& light: scene.lights())
    {
        if(light.type == cn::light::light_type::directional)
        {
            active_light = &light;
            break;
        }
    }

    float light_dir[4] = {0, -1, 0, 0};
    float light_color[4] = {1, 1, 1, 1};
    float light_intensity[4] = {1.0f, 0, 0, 0};

    if(active_light)
    {
        const auto& dir = active_light->direction;
        const auto& col = active_light->color;
        light_dir[0] = dir[0];
        light_dir[1] = dir[1];
        light_dir[2] = dir[2];

        light_color[0] = col[0];
        light_color[1] = col[1];
        light_color[2] = col[2];
        light_color[3] = col[3];

        light_intensity[0] = active_light->intensity;
    }

    for(const auto& drawable: scene.views())
    {
        const float* model = reinterpret_cast<const float*>(drawable->transform().data());
        bgfx::setTransform(model);
        bgfx::setVertexBuffer(0, drawable->get_model().vertex_buffer());
        bgfx::setIndexBuffer(drawable->get_model().index_buffer());

        // Set light uniforms
        bgfx::setUniform(u_lightDir, light_dir);
        bgfx::setUniform(u_lightColor, light_color);
        bgfx::setUniform(u_lightIntensity, light_intensity);

        // Set material uniforms
        for(const auto& [name, entry]: drawable->get_material().uniforms())
        {
            bgfx::setUniform(entry.handle, entry.value.data());
        }

        for(std::uint8_t i = 0; const auto& texture: drawable->get_material().textures())
        {
            bgfx::setTexture(i++, texture.uniform, texture.handle);
        }

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(m_main_view, drawable->get_material().shader_program());
    }
}

void renderer::draw_postprocess(std::string_view effect_name)
{
    if(effect_name == "fxaa")
    {
        // Apply FXAA (post-processing effect)
        // Requires fullscreen quad and FXAA shader setup
        // bgfx::setTexture, setUniform, submit...
    }
}

void renderer::end_frame()
{
    bgfx::frame();
}

} // namespace cn
