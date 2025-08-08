#include <cn/graphics/material.hpp>
#include <cn/graphics/shader.hpp>

namespace cn
{

material::~material()
{
    // We do not own the uniforms/textures — shader handles their destruction
}

void material::set(std::string_view name, const cn::color& color)
{
    std::array<float, 4> vec = {
        static_cast<float>(color[0]),
        static_cast<float>(color[1]),
        static_cast<float>(color[2]),
        static_cast<float>(color[3])};

    m_uniforms.push_back({std::string(name), bgfx::UniformType::Vec4, vec});
}

void material::set(std::string_view name, float value)
{
    m_uniforms.push_back({std::string(name), bgfx::UniformType::Vec4, {value, 0.0f, 0.0f, 0.0f}});
}

void material::set(std::string_view name, bgfx::TextureHandle texture)
{
    m_textures.push_back({std::string(name), texture});
}

void material::apply(shader& shader)
{
    for(const auto& entry: m_uniforms)
    {
        auto handle = shader.ensure_uniform(entry.name, entry.type);
        bgfx::setUniform(handle, entry.value.data());
    }

    uint8_t slot = 0;
    for(const auto& texture: m_textures)
    {
        auto handle = shader.ensure_uniform(texture.name, bgfx::UniformType::Sampler);
        bgfx::setTexture(slot++, handle, texture.handle);
    }
}

} // namespace cn
