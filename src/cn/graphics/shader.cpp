#include <bgfx/bgfx.h>
#include <cn/graphics/shader.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace cn
{

namespace
{

bgfx::ShaderHandle load_shader(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::binary);

    if(!file)
    {
        std::cerr << "Failed to open shader file: " << path << '\n';
        return BGFX_INVALID_HANDLE;
    }

    std::vector<uint8_t> buffer{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()};

    buffer.push_back('\0'); // Ensure null-terminated

    const bgfx::Memory* mem = bgfx::copy(buffer.data(), static_cast<uint32_t>(buffer.size()));
    bgfx::ShaderHandle  handle = bgfx::createShader(mem);

    if(bgfx::isValid(handle))
    {
        bgfx::setName(handle, path.data());
    }
    else
    {
        std::cerr << "Failed to create shader: " << path << '\n';
    }

    return handle;
}

} // namespace

shader shader::load(std::string_view vs_path, std::string_view fs_path)
{
    auto vsh = load_shader(vs_path);
    if(!bgfx::isValid(vsh))
    {
        std::cerr << "Vertex shader is invalid!\n";
    }

    auto fsh = load_shader(fs_path);
    if(!bgfx::isValid(fsh))
    {
        std::cerr << "Fragment shader is invalid!\n";
    }

    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);
    if(!bgfx::isValid(program))
    {
        std::cerr << "Failed to create shader program!\n";
    }

    return shader{program};
}

shader::shader(bgfx::ProgramHandle program) noexcept : m_program(program) {}

shader::~shader()
{
    if(bgfx::isValid(m_program))
    {
        bgfx::destroy(m_program);
    }
}

bgfx::ProgramHandle shader::handle() const noexcept
{
    return m_program;
}

void shader::submit(bgfx::ViewId view) const noexcept
{
    if(bgfx::isValid(m_program))
    {
        bgfx::submit(view, m_program);
    }
}

} // namespace cn
