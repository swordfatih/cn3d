#pragma once

#include <bgfx/bgfx.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

namespace cn
{

class shader
{
public:
    shader() noexcept = default;
    ~shader();

    static shader load(std::filesystem::path path);

    bgfx::ProgramHandle handle() const noexcept;
    void                submit(bgfx::ViewId view) const noexcept;

    bgfx::UniformHandle ensure_uniform(std::string name, bgfx::UniformType::Enum type, uint16_t count = 1);

private:
    explicit shader(bgfx::ProgramHandle program) noexcept;

    bgfx::ProgramHandle                                  m_program{BGFX_INVALID_HANDLE};
    std::unordered_map<std::string, bgfx::UniformHandle> m_uniforms;
};

} // namespace cn
