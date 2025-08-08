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

    auto shader = cn::shader::load("../../../shaders/cube");
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
        renderer.draw(plane, shader, glm::mat4(1.0f));
        renderer.draw(box, shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));
        renderer.end_frame();
    }

    return 0;
}