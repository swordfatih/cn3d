#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

// ─────────────────────────── Vertex format ────────────────────────────
struct PosColorVertex
{
    float x, y, z;
    uint32_t abgr;
    static void init(bgfx::VertexLayout& layout)
    {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
    }
};

// Cube geometry
static PosColorVertex vertices[] =
{
    // +X
    {-1,  1,  1, 0xff0000ff}, { 1,  1,  1, 0xff0000ff},
    {-1, -1,  1, 0xff0000ff}, { 1, -1,  1, 0xff0000ff},
    // -X
    {-1,  1, -1, 0xff00ff00}, { 1,  1, -1, 0xff00ff00},
    {-1, -1, -1, 0xff00ff00}, { 1, -1, -1, 0xff00ff00},
};

static const uint16_t indices[] =
{
    0, 1, 3, 0, 3, 2,   // front
    4, 6, 7, 4, 7, 5,   // back
    4, 5, 1, 4, 1, 0,   // top
    2, 3, 7, 2, 7, 6,   // bottom
    0, 2, 6, 0, 6, 4,   // left
    1, 5, 7, 1, 7, 3    // right
};

// Helper: load a binary shader file
bgfx::ShaderHandle loadShader(const char* path)
{
    FILE* fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    const long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    const bgfx::Memory* mem = bgfx::alloc(size + 1);
    fread(mem->data, 1, size, fp);
    fclose(fp);
    mem->data[mem->size-1] = '\0';
    return bgfx::createShader(mem);
}

int main()
{
    // ───────────── GLFW window ──────────────
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "BGFX Cube", nullptr, nullptr);

    // ───────────── BGFX init ───────────────
    bgfx::PlatformData pd{};
#if BX_PLATFORM_WINDOWS
    pd.nwh = glfwGetWin32Window(window);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.nwh = glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    pd.nwh = glfwGetCocoaWindow(window);
#endif
    bgfx::Init init{};
    init.type = bgfx::RendererType::Count;   // autodetect
    init.platformData = pd;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    init.resolution.width  = static_cast<uint32_t>(width);
    init.resolution.height = static_cast<uint32_t>(height);
    init.resolution.reset  = BGFX_RESET_VSYNC;
    bgfx::init(init);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // ───────────── Geometry buffers ─────────
    bgfx::VertexLayout layout;
    PosColorVertex::init(layout);
    bgfx::VertexBufferHandle vbo =
        bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);
    bgfx::IndexBufferHandle  ibo =
        bgfx::createIndexBuffer(bgfx::makeRef(indices,  sizeof(indices)));

    // ───────────── Shaders & program ───────
    bgfx::ShaderHandle vsh = loadShader("debug.vert.bin");
    bgfx::ShaderHandle fsh = loadShader("debug.frag.bin");
    bgfx::ProgramHandle prog = bgfx::createProgram(vsh, fsh, true); // shaders auto-destroy

    // ───────────── View transform (static) ─
    float view[16];
    float proj[16];
    const bx::Vec3 eye    = { 0.0f, 0.0f, -7.0f };
    const bx::Vec3 at     = { 0.0f, 0.0f,  0.0f };
    const bx::Vec3 up     = { 0.0f, 1.0f,  0.0f };
    bx::mtxLookAt(view, eye, at, up);
    bx::mtxProj(proj, 60.0f, float(width)/float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);
    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

    // ───────────── Main loop ───────────────
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Clear view 0.
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        // Model matrix (identity)
        float mtx[16];
        bx::mtxRotateY(mtx, (float)glfwGetTime()); // a slow spin
        bgfx::setTransform(mtx);

        // Bind buffers & submit
        bgfx::setVertexBuffer(0, vbo);
        bgfx::setIndexBuffer(ibo);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, prog);

        // Frame
        bgfx::frame();
    }

    // ───────────── Shutdown ────────────────
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
