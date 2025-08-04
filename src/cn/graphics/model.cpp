#include <bgfx/bgfx.h>
#include <cn/graphics/model.hpp>
#include <cstring>
#include <glm/glm.hpp>
#include <iostream>
#include <tiny_gltf.h>
#include <vector>

namespace cn
{

namespace
{

bgfx::VertexLayout make_position_layout()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();
    return layout;
}

} // namespace

model::model(bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer)
    : m_vertex_buffer(vertex_buffer), m_index_buffer(index_buffer)
{
    if(!bgfx::isValid(m_vertex_buffer) || !bgfx::isValid(m_index_buffer))
    {
        std::cerr << "Error: Invalid vertex or index buffer" << std::endl;
    }
}

model model::make_plane(const cn::math::vector<std::float32_t, 2>& size)
{
    std::vector<glm::vec3> vertices = {
        {-size[0] / 2.0f, 0.0f, -size[1] / 2.0f},
        {size[0] / 2.0f, 0.0f, -size[1] / 2.0f},
        {-size[0] / 2.0f, 0.0f, size[1] / 2.0f},
        {size[0] / 2.0f, 0.0f, size[1] / 2.0f}};

    std::vector<uint16_t> indices = {0, 1, 2, 1, 3, 2};

    bgfx::VertexLayout layout = make_position_layout();

    const bgfx::Memory* vb_mem = bgfx::copy(vertices.data(), uint32_t(vertices.size() * sizeof(glm::vec3)));
    const bgfx::Memory* ib_mem = bgfx::copy(indices.data(), uint32_t(indices.size() * sizeof(uint16_t)));

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(vb_mem, layout);
    bgfx::IndexBufferHandle  ibh = bgfx::createIndexBuffer(ib_mem);

    return model(vbh, ibh);
}

model model::make_cube(const cn::math::vector<std::float32_t, 3>& size)
{
    float hx = size[0] * 0.5f;
    float hy = size[1] * 0.5f;
    float hz = size[2] * 0.5f;

    std::vector<glm::vec3> vertices = {
        {-hx, -hy, -hz},
        {hx, -hy, -hz},
        {hx, hy, -hz},
        {-hx, hy, -hz},
        {-hx, -hy, hz},
        {hx, -hy, hz},
        {hx, hy, hz},
        {-hx, hy, hz},
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0, // back
        4, 5, 6, 6, 7, 4, // front
        0, 4, 7, 7, 3, 0, // left
        1, 5, 6, 6, 2, 1, // right
        3, 2, 6, 6, 7, 3, // top
        0, 1, 5, 5, 4, 0, // bottom
    };

    bgfx::VertexLayout layout = make_position_layout();

    const bgfx::Memory* vb_mem = bgfx::copy(vertices.data(), uint32_t(vertices.size() * sizeof(glm::vec3)));
    const bgfx::Memory* ib_mem = bgfx::copy(indices.data(), uint32_t(indices.size() * sizeof(uint16_t)));

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(vb_mem, layout);
    bgfx::IndexBufferHandle  ibh = bgfx::createIndexBuffer(ib_mem);

    return model(vbh, ibh);
}

model model::load_from_file(std::string_view path)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model    gltf_model;
    std::string        err, warn;

    bool success = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, std::string(path));
    if(!success)
    {
        std::cerr << "Failed to load GLTF: " << err << "\n";
        return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
    }

    if(!warn.empty())
    {
        std::cout << "GLTF warning: " << warn << "\n";
    }

    if(gltf_model.meshes.empty())
    {
        std::cerr << "No meshes found in GLTF.\n";
        return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
    }

    const auto& mesh = gltf_model.meshes[0];
    if(mesh.primitives.empty())
    {
        std::cerr << "No primitives in mesh.\n";
        return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
    }

    const auto& primitive = mesh.primitives[0];

    if(!primitive.attributes.contains("POSITION"))
    {
        std::cerr << "Missing POSITION attribute.\n";
        return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
    }

    const auto& pos_accessor = gltf_model.accessors[primitive.attributes.at("POSITION")];
    const auto& pos_view = gltf_model.bufferViews[pos_accessor.bufferView];
    const auto& pos_buffer = gltf_model.buffers[pos_view.buffer];

    std::vector<glm::vec3> vertices;
    vertices.reserve(pos_accessor.count);

    const uint8_t* pos_data = pos_buffer.data.data() + pos_view.byteOffset + pos_accessor.byteOffset;
    for(size_t i = 0; i < pos_accessor.count; ++i)
    {
        const float* f = reinterpret_cast<const float*>(pos_data + i * sizeof(glm::vec3));
        vertices.emplace_back(f[0], f[1], f[2]);
    }

    std::vector<uint16_t> indices;
    if(primitive.indices >= 0)
    {
        const auto& idx_accessor = gltf_model.accessors[primitive.indices];
        const auto& idx_view = gltf_model.bufferViews[idx_accessor.bufferView];
        const auto& idx_buffer = gltf_model.buffers[idx_view.buffer];

        const uint8_t* idx_data = idx_buffer.data.data() + idx_view.byteOffset + idx_accessor.byteOffset;
        indices.reserve(idx_accessor.count);

        for(size_t i = 0; i < idx_accessor.count; ++i)
        {
            indices.push_back(reinterpret_cast<const uint16_t*>(idx_data)[i]);
        }
    }
    else
    {
        std::cerr << "Warning: No indices found in primitive.\n";
        // If no indices, we could generate them here — optional.
    }

    bgfx::VertexLayout layout = make_position_layout();

    const bgfx::Memory* vb_mem = bgfx::copy(vertices.data(), uint32_t(vertices.size() * sizeof(glm::vec3)));
    const bgfx::Memory* ib_mem = bgfx::copy(indices.data(), uint32_t(indices.size() * sizeof(uint16_t)));

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(vb_mem, layout);
    bgfx::IndexBufferHandle  ibh = bgfx::createIndexBuffer(ib_mem);

    return model(vbh, ibh);
}

bgfx::VertexBufferHandle model::vertex_buffer() const
{
    return m_vertex_buffer;
}

bgfx::IndexBufferHandle model::index_buffer() const
{
    return m_index_buffer;
}

} // namespace cn
