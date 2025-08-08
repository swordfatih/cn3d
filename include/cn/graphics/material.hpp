#pragma once

#include <bgfx/bgfx.h>
#include <array>
#include <cn/color/color.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cn
{

class shader;

class material
{
public:
    material() = default;
    ~material();

    void set(std::string_view name, const cn::color& color);
    void set(std::string_view name, float value);
    void set(std::string_view name, bgfx::TextureHandle texture);

    void apply(shader& shader);

private:
    struct uniform_entry
    {
        std::string name;
        bgfx::UniformType::Enum type;
        std::array<float, 4> value{};
    };

    struct texture_entry
    {
        std::string name;
        bgfx::TextureHandle handle{};
    };

    std::vector<uniform_entry> m_uniforms;
    std::vector<texture_entry> m_textures;
};

} // namespace cn
