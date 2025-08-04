#pragma once

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <string_view>

namespace cn
{

class window;
class scene;
class camera;

class renderer
{
public:
    explicit renderer(cn::window& window, bgfx::RendererType::Enum type);
    ~renderer();

    void begin_frame();
    void draw_depth_pass(const cn::scene& scene, const cn::camera& camera);
    void render(const cn::scene& scene, const cn::camera& camera);
    void draw_postprocess(std::string_view effect_name);
    void end_frame();

private:
    cn::window& m_window;

    bgfx::ViewId m_main_view = 0;
    bgfx::ViewId m_depth_view = 1;
    bgfx::ViewId m_post_view = 2;

    bgfx::UniformHandle u_lightDir;
    bgfx::UniformHandle u_lightColor;
    bgfx::UniformHandle u_lightIntensity;
};

} // namespace cn
