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

    cn::camera camera;

    auto shader = cn::shader::load("shaders/vs_cubes.bin", "shaders/fs_cubes.bin");
    auto box = cn::model::make_cube({1.0f, 1.0f, 1.0f});
    auto plane = cn::model::make_plane({10.0f, 10.0f});

    float angle = 0.0f;
    float radius = 7.0f;

    while(window.is_open())
    {
        cn::time::update();
        window.poll_events();

        angle += 0.5f * cn::time::delta();

        float x = std::sin(angle) * radius;
        float z = std::cos(angle) * radius;

        camera.look_at({x, 5.0f, z}, {0.0f, 0.0f, 0.0f});

        glm::mat4 plane_transform = glm::mat4(1.0f);
        glm::mat4 cube_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));

        renderer.begin_frame(camera);
        renderer.draw(plane, shader, plane_transform);
        renderer.draw(box, shader, cube_transform);
        renderer.end_frame();
    }

    return 0;
}