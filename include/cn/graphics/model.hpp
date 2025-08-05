#pragma once

#include <bgfx/bgfx.h>
#include <cn/math/vector.hpp>
#include <stdfloat>
#include <string_view>
#include <tiny_gltf.h>

namespace cn
{

class model
{
public:
    model(bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer);

    static model load_from_file(std::string_view path);

    bgfx::VertexBufferHandle vertex_buffer() const;
    bgfx::IndexBufferHandle  index_buffer() const;

private:
    bgfx::VertexBufferHandle m_vertex_buffer;
    bgfx::IndexBufferHandle  m_index_buffer;
};

} // namespace cn
