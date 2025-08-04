#pragma once

#include <bgfx/bgfx.h>
#include <string_view>

namespace cn
{

class shader
{
public:
    shader() noexcept = default;
    ~shader();

    [[nodiscard]] static shader       load(std::string_view vs_path, std::string_view fs_path);
    [[nodiscard]] bgfx::ProgramHandle handle() const noexcept;
    void                              submit(bgfx::ViewId view) const noexcept;

private:
    explicit shader(bgfx::ProgramHandle program) noexcept;

    bgfx::ProgramHandle m_program{BGFX_INVALID_HANDLE};
};

} // namespace cn
