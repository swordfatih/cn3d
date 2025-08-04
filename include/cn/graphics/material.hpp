#pragma once

#include <array>
#include <bgfx/bgfx.h>
#include <cn/color/color.hpp>
#include <cn/graphics/shader.hpp>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cn
{

class material
{
public:
    explicit material(const shader& shader_program, const shader& depth_program = shader{});
    ~material();

    void set(std::string_view name, const cn::color& color);
    void set(std::string_view name, float value);
    void set(std::string_view name, bgfx::TextureHandle texture);

    bgfx::ProgramHandle shader_program() const;
    bgfx::ProgramHandle depth_program() const;

    struct uniform_entry
    {
        bgfx::UniformHandle  handle{};
        std::array<float, 4> value{};
    };

    struct texture_binding
    {
        bgfx::UniformHandle uniform{};
        bgfx::TextureHandle handle{};
    };

    const std::unordered_map<std::string, uniform_entry>& uniforms() const;
    const std::vector<texture_binding>&                   textures() const;

private:
    bgfx::UniformHandle get_uniform(std::string_view name, bgfx::UniformType::Enum type, std::uint16_t count = 1);

    shader                                               m_shader_program;
    shader                                               m_depth_program;
    std::unordered_map<std::string, uniform_entry>       m_uniforms;
    std::vector<texture_binding>                         m_textures;
    std::unordered_map<std::string, bgfx::UniformHandle> m_uniform_cache;
};

} // namespace cn
