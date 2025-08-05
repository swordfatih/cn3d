#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cstdint>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex format
struct PosColorVertex {
    float x, y, z;
    uint32_t abgr;

    static void init(bgfx::VertexLayout& layout) {
        layout.begin()
              .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
              .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
              .end();
    }
};

// Geometry data
static constexpr PosColorVertex cubeVertices[] = {
    {-1, 1, 1, 0xff0000ff}, {1, 1, 1, 0xff0000ff},
    {-1, -1, 1, 0xff0000ff}, {1, -1, 1, 0xff0000ff},
    {-1, 1, -1, 0xff00ff00}, {1, 1, -1, 0xff00ff00},
    {-1, -1, -1, 0xff00ff00}, {1, -1, -1, 0xff00ff00},
};

static constexpr uint16_t cubeIndices[] = {
    0, 1, 3, 0, 3, 2,
    4, 6, 7, 4, 7, 5,
    4, 5, 1, 4, 1, 0,
    2, 3, 7, 2, 7, 6,
    0, 2, 6, 0, 6, 4,
    1, 5, 7, 1, 7, 3,
};

// Shader loader
bgfx::ShaderHandle loadShader(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader: " << filePath << '\n';
        return BGFX_INVALID_HANDLE;
    }

    const auto size = static_cast<std::streamsize>(file.tellg());
    file.seekg(0, std::ios::beg);

    const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size + 1));
    if (!file.read(reinterpret_cast<char*>(mem->data), size)) {
        std::cerr << "Failed to read shader: " << filePath << '\n';
        return BGFX_INVALID_HANDLE;
    }

    mem->data[mem->size - 1] = '\0';
    return bgfx::createShader(mem);
}

int main() {
    // SFML window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::Window window(sf::VideoMode({1280, 720}), "BGFX + SFML", sf::Style::Default, sf::State::Windowed, settings);

    // BGFX platform setup
    bgfx::PlatformData pd{};
    pd.nwh = (void*)window.getNativeHandle();
    pd.ndt = nullptr;

    bgfx::Init init;
    init.type = bgfx::RendererType::Direct3D11;
    init.platformData = pd;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize BGFX.\n";
        return 1;
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    // Buffers
    bgfx::VertexLayout layout;
    PosColorVertex::init(layout);

    const bgfx::Memory* vbMem = bgfx::copy(cubeVertices, sizeof(cubeVertices));
    bgfx::VertexBufferHandle vbo = bgfx::createVertexBuffer(vbMem, layout);

    const bgfx::Memory* ibMem = bgfx::copy(cubeIndices, sizeof(cubeIndices));
    bgfx::IndexBufferHandle ibo = bgfx::createIndexBuffer(ibMem);

    // Shader program
    auto vsh = loadShader("shaders/vs_cubes.bin");
    auto fsh = loadShader("shaders/fs_cubes.bin");

    if (!bgfx::isValid(vsh) || !bgfx::isValid(fsh)) return 1;

    auto prog = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(prog)) return 1;

    // Camera setup
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -7.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 100.0f);

    bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    // Model is identity (no rotation)
    glm::mat4 model(1.0f);
    bgfx::setTransform(glm::value_ptr(model));

    // Main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        bgfx::touch(0);
        bgfx::setVertexBuffer(0, vbo);
        bgfx::setIndexBuffer(ibo);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, prog);
        bgfx::frame();
    }

    bgfx::shutdown();
    return 0;
}
