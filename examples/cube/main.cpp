#include <cn/camera/camera.hpp>
#include <cn/color/color.hpp>
#include <cn/graphics/light.hpp>
#include <cn/graphics/material.hpp>
#include <cn/graphics/model.hpp>
#include <cn/graphics/renderer.hpp>
#include <cn/graphics/shader.hpp>
#include <cn/input/input.hpp>
#include <cn/math/vector.hpp>
#include <cn/scene/scene.hpp>
#include <cn/scene/view.hpp>
#include <cn/time/time.hpp>
#include <cn/window/window.hpp>
#include <iostream>

int main()
{
    cn::window   window{{1280, 720}, "cn3d"};
    cn::renderer renderer{window, bgfx::RendererType::Direct3D11};
    cn::scene    scene;

    bgfx::RendererType::Enum render_type = bgfx::getRendererType();
    std::cout << "Using renderer: " << render_type << "\n";

    cn::camera camera;
    camera.set_position({0.0f, 2.0f, 6.0f});
    camera.look_at({0.0f, 0.0f, 0.0f});
    camera.set_aspect_ratio(static_cast<float>(window.size()[0]) / window.size()[1]);

    // shader and materials
    std::cout << "Loading shaders..." << std::endl;
    auto lit_shader = cn::shader::load("shaders/lit.vert.bin", "shaders/lit.frag.bin");
    std::cout << "Loaded shader: " << lit_shader.handle().idx << std::endl;

    cn::material plane_mat{lit_shader};
    plane_mat.set("base_color", cn::colors::gray);
    plane_mat.set("roughness", 1.0f);

    cn::material cube_mat{lit_shader};
    cube_mat.set("base_color", cn::colors::white);
    cube_mat.set("metallic", 0.1f);

    // models
    auto plane = cn::model::make_plane({10.0f, 10.0f});
    // auto box = cn::model::load_from_file("assets/box.gltf");
    auto box = cn::model::make_cube({1.0f, 1.0f, 1.0f});

    auto& plane_view = scene.add(plane);
    plane_view.set_position({0.0f, -0.5f, 0.0f});
    plane_view.set_material(std::move(plane_mat));

    auto& box_view = scene.add(box);
    box_view.set_position({0.0f, 0.0f, 0.0f});
    box_view.set_material(std::move(cube_mat));

    // lights
    scene.add_light(cn::light::make_directional({-1.0f, -2.0f, -1.0f}, cn::colors::white, 1.0f));
    scene.add_light(cn::light::make_point({2.0f, 3.0f, 2.0f}, cn::colors::warm_white, 3.0f));

    float camera_speed = 5.0f;
    float look_sensitivity = 0.002f;

    // --- input axis mapping ---
    cn::input::map_axis("move_x", cn::key::a, cn::key::d); // left/right
    cn::input::map_axis("move_y", cn::key::q, cn::key::e); // down/up
    cn::input::map_axis("move_z", cn::key::s, cn::key::w); // back/forward

    bool looking = false;

    while(window.is_open())
    {
        window.poll_events();
        cn::time::update();
        float dt = cn::time::delta();

        // --- camera movement ---
        cn::math::vector<std::float64_t, 3> move_input{
            cn::input::axis("move_x"),
            cn::input::axis("move_y"),
            cn::input::axis("move_z"),
        };

        camera.translate_local(move_input * dt * camera_speed);

        // --- mouse orbit ---
        if(cn::input::pressed(cn::mouse::right))
        {
            window.capture_mouse(true);
            looking = true;
        }

        if(cn::input::released(cn::mouse::right))
        {
            window.capture_mouse(false);
            looking = false;
        }

        if(looking)
        {
            cn::math::vector<std::float32_t, 2> delta = cn::input::mouse_delta();
            camera.rotate_view({
                -delta[0] * look_sensitivity,
                -delta[1] * look_sensitivity,
                0.0f,
            });
        }

        // --- cube interaction ---
        if(cn::input::pressed(cn::key::space))
        {
            // box_view.translate({0.0f, 1.0f, 0.0f});
        }

        // --- scene update & render ---
        scene.update_all(dt);

        renderer.begin_frame();
        renderer.draw_depth_pass(scene, camera);
        renderer.render(scene, camera);
        renderer.draw_postprocess("fxaa");
        renderer.end_frame();
    }

    return 0;
}