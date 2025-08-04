#include <cn/graphics/material.hpp>

namespace cn
{

material::material(const shader& shader_program, const shader& depth_program) : m_shader_program(shader_program), m_depth_program(depth_program) {}

material::~material()
{
    for(auto& [_, entry]: m_uniforms)
    {
        if(bgfx::isValid(entry.handle))
        {
            bgfx::destroy(entry.handle);
        }
    }

    for(const auto& [_, handle]: m_uniform_cache)
    {
        if(bgfx::isValid(handle))
        {
            bgfx::destroy(handle);
        }
    }

    for(const auto& tex: m_textures)
    {
        if(bgfx::isValid(tex.uniform))
        {
            bgfx::destroy(tex.uniform);
        }
    }
}

void material::set(std::string_view name, const cn::color& color)
{
    auto& entry = m_uniforms.emplace(std::string(name), uniform_entry{}).first->second;

    if(!bgfx::isValid(entry.handle))
    {
        entry.handle = get_uniform(name, bgfx::UniformType::Vec4);
    }

    entry.value = {static_cast<float>(color[0]), static_cast<float>(color[1]), static_cast<float>(color[2]), static_cast<float>(color[3])};
}

void material::set(std::string_view name, float value)
{
    auto& entry = m_uniforms.emplace(std::string(name), uniform_entry{}).first->second;
    if(!bgfx::isValid(entry.handle))
    {
        entry.handle = get_uniform(name, bgfx::UniformType::Vec4);
    }
    entry.value = {value, 0.0f, 0.0f, 0.0f};
}

void material::set(std::string_view name, bgfx::TextureHandle texture)
{
    bgfx::UniformHandle sampler = get_uniform(name, bgfx::UniformType::Sampler);
    m_textures.push_back({.uniform = sampler, .handle = texture});
}

bgfx::ProgramHandle material::shader_program() const
{
    return m_shader_program.handle();
}

bgfx::ProgramHandle material::depth_program() const
{
    return m_depth_program.handle();
}

const std::unordered_map<std::string, material::uniform_entry>& material::uniforms() const
{
    return m_uniforms;
}

const std::vector<material::texture_binding>& material::textures() const
{
    return m_textures;
}

bgfx::UniformHandle material::get_uniform(std::string_view name, bgfx::UniformType::Enum type, std::uint16_t count)
{
    std::string key{name};
    auto        it = m_uniform_cache.find(key);

    if(it != m_uniform_cache.end())
    {
        return it->second;
    }

    bgfx::UniformHandle handle = bgfx::createUniform(key.c_str(), type, count);
    m_uniform_cache.emplace(std::move(key), handle);
    return handle;
}

} // namespace cn
