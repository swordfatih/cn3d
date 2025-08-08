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

void material::set(std::string_view name, const cn::math::vector<float, 3>& vec3)
{
    std::array<float, 4> vec = {vec3[0], vec3[1], vec3[2], 0.0f};
    m_uniforms.push_back({std::string(name), bgfx::UniformType::Vec4, vec});
}

void material::set(std::string_view name, const cn::math::vector<float, 4>& vec4)
{
    std::array<float, 4> vec = {vec4[0], vec4[1], vec4[2], vec4[3]};
    m_uniforms.push_back({std::string(name), bgfx::UniformType::Vec4, vec});
}

void material::load_texture(std::string_view uniform_name, const std::filesystem::path& path)
{
    if(!std::filesystem::exists(path))
    {
        throw std::runtime_error("Texture file not found: " + path.string());
    }

    int      width, height, channels;
    uint8_t* pixels = stbi_load(path.string().c_str(), &width, &height, &channels, 4);

    bgfx::TextureHandle texture = bgfx::createTexture2D(
        (uint16_t)width,
        (uint16_t)height,
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        0,
        bgfx::copy(pixels, width * height * 4));

    stbi_image_free(pixels);

    set(uniform_name, texture);
}


} // namespace cn
