#include <cn/camera/camera.hpp>
#include <cn/color/color.hpp>
#include <cn/graphics/light.hpp>
#include <cn/graphics/material.hpp>
#include <cn/graphics/model.hpp>
#include <cn/graphics/renderer.hpp>
#include <cn/graphics/shader.hpp>
#include <cn/input/input.hpp>
#include <cn/math/vector.hpp>
#include <cn/time/time.hpp>
#include <cn/window/window.hpp>
#include <iostream>

int main()
{
    cn::window   window{{1280, 720}, "cn3d"};
    cn::renderer renderer{window, bgfx::RendererType::Direct3D11};

    cn::camera camera;

    auto shader = cn::shader::load("../../../shaders/pbr");
    shader.ensure_uniform("u_base_color", bgfx::UniformType::Vec4);
    shader.ensure_uniform("u_metallic", bgfx::UniformType::Vec4);
    shader.ensure_uniform("u_roughness", bgfx::UniformType::Vec4);
    shader.ensure_uniform("u_base_color_tex", bgfx::UniformType::Sampler);
    shader.ensure_uniform("u_roughness_tex", bgfx::UniformType::Sampler);
    shader.ensure_uniform("u_metallic_tex", bgfx::UniformType::Sampler);
    shader.ensure_uniform("u_light_dir", bgfx::UniformType::Vec4);
    shader.ensure_uniform("u_light_color", bgfx::UniformType::Vec4);

    cn::material metal;
    metal.load_texture("u_base_color_tex", "assets/metal_basecolor.png");
    metal.load_texture("u_roughness_tex", "assets/metal_roughness.png");
    metal.load_texture("u_metallic_tex", "assets/metal_metalness.png");
    metal.set("u_base_color", cn::color{0.8f, 0.8f, 0.8f, 1.0f});
    metal.set("u_metallic", 1.0f);
    metal.set("u_roughness", 0.1f);
    metal.set("u_light_dir", cn::math::vector<float, 3>{-0.577f, -0.577f, -0.577f});
    metal.set("u_light_color", cn::color{1.0f, 1.0f, 1.0f, 1.0f});

    auto plane = cn::model::load_from_file("assets/plane.glb");
    auto box = cn::model::load_from_file("assets/cube.glb");

    float angle = 0.0f;
    while(window.is_open())
    {
        cn::time::update();
        window.poll_events();

        angle += 0.5f * cn::time::delta();
        camera.orbit({0.0f, 0.0f, 0.0f}, 7.0f, angle, 5.0f);

        renderer.begin_frame(camera);
        renderer.draw(plane, shader, metal, glm::mat4(1.0f));
        renderer.draw(box, shader, metal, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));
        renderer.end_frame();
    }

    return 0;
}