#include <cn/graphics/shader.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace cn
{

namespace
{

bgfx::ShaderHandle load_shader(std::filesystem::path path)
{
    if(!std::filesystem::exists(path))
    {
        throw std::runtime_error{"Shader file does not exist: " + path.string()};
    }

    std::ifstream file{path, std::ios::binary};

    if(!file)
    {
        throw std::runtime_error{"Failed to open shader file: " + path.string()};
    }

    std::vector<uint8_t> buffer{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    buffer.push_back('\0');

    const bgfx::Memory* mem = bgfx::copy(buffer.data(), static_cast<uint32_t>(buffer.size()));
    auto                handle = bgfx::createShader(mem);

    if(bgfx::isValid(handle))
    {
        bgfx::setName(handle, path.string().c_str());
    }

    return handle;
}

} // namespace

shader shader::load(std::filesystem::path path)
{
    if(!std::filesystem::exists(path))
    {
        throw std::runtime_error{"Shader folder does not exist: " + path.string()};
    }

    const auto vsh = load_shader(path / "vs.bin");
    const auto fsh = load_shader(path / "fs.bin");

    if(!bgfx::isValid(vsh) || !bgfx::isValid(fsh))
    {
        std::cerr << "Shader load failed: " << (path / "vs.bin") << " or " << (path / "fs.bin") << '\n';
    }

    return shader{bgfx::createProgram(vsh, fsh, true)};
}

shader::shader(bgfx::ProgramHandle program) noexcept : m_program(program) {}

shader::~shader()
{
    for(auto& [_, handle]: m_uniforms)
    {
        if(bgfx::isValid(handle))
        {
            bgfx::destroy(handle);
        }
    }

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
        bgfx::submit(view, m_program);
}

bgfx::UniformHandle shader::ensure_uniform(std::string name, bgfx::UniformType::Enum type, uint16_t count)
{
    if(!m_uniforms.contains(name))
    {
        m_uniforms[name] = bgfx::createUniform(name.data(), type, count);
    }

    return m_uniforms.at(name);
}

} // namespace cn
