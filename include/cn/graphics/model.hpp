#pragma once

#include <bgfx/bgfx.h>
#include <cn/math/vector.hpp>
#include <stdfloat>
#include <string_view>
#include <vector>

namespace cn
{

class model
{
public:
    model(bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer);

    static model make_plane(const cn::math::vector<std::float32_t, 2>& size);
    static model make_cube(const cn::math::vector<std::float32_t, 3>& size);
    static model load_from_file(std::string_view path);

    bgfx::VertexBufferHandle vertex_buffer() const;
    bgfx::IndexBufferHandle  index_buffer() const;

private:
    bgfx::VertexBufferHandle m_vertex_buffer;
    bgfx::IndexBufferHandle  m_index_buffer;
};

} // namespace cn
