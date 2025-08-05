#include <bgfx/bgfx.h>
#include <cn/graphics/model.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string_view>
#include <tiny_gltf.h>
#include <variant>
#include <vector>

namespace cn
{

namespace
{

struct vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

bgfx::VertexLayout make_vertex_layout()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
        .end();
    return layout;
}

template <typename t>
const t* get_data_pointer(const tinygltf::BufferView& view, const tinygltf::Accessor& accessor, const tinygltf::Buffer& buffer)
{
    auto byte_offset = view.byteOffset + accessor.byteOffset;
    return reinterpret_cast<const t*>(buffer.data.data() + byte_offset);
}

std::vector<vertex> extract_vertices(const tinygltf::Model& gltf, const tinygltf::Primitive& primitive)
{
    std::vector<vertex> vertices;

    const auto& accessor = gltf.accessors.at(primitive.attributes.at("POSITION"));
    const auto& view = gltf.bufferViews.at(accessor.bufferView);
    const auto& buffer = gltf.buffers.at(view.buffer);

    std::size_t         count = accessor.count;
    std::size_t         stride = view.byteStride ? view.byteStride : sizeof(glm::vec3);
    const std::uint8_t* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

    // 🆕 Get base color from material
    glm::vec4 base_color{0.8f, 0.8f, 0.8f, 1.0f};

    if(primitive.material >= 0 && static_cast<std::size_t>(primitive.material) < gltf.materials.size())
    {
        const auto& material = gltf.materials[primitive.material];
        const auto& pbr = material.pbrMetallicRoughness;

        if(!pbr.baseColorFactor.empty())
        {
            base_color = glm::vec4(
                static_cast<float>(pbr.baseColorFactor[0]),
                static_cast<float>(pbr.baseColorFactor[1]),
                static_cast<float>(pbr.baseColorFactor[2]),
                static_cast<float>(pbr.baseColorFactor[3]));
        }
    }

    vertices.reserve(count);
    for(std::size_t i = 0; i < count; ++i)
    {
        const float* position_ptr = reinterpret_cast<const float*>(data + i * stride);
        glm::vec3    position = {position_ptr[0], position_ptr[1], position_ptr[2]};
        vertices.push_back({position, base_color});
    }

    return vertices;
}

std::vector<std::uint16_t> extract_indices(const tinygltf::Model& gltf, const tinygltf::Primitive& primitive)
{
    std::vector<std::uint16_t> indices;

    if(primitive.indices < 0)
        return indices;

    const auto& accessor = gltf.accessors.at(primitive.indices);
    const auto& view = gltf.bufferViews.at(accessor.bufferView);
    const auto& buffer = gltf.buffers.at(view.buffer);

    const std::size_t   count = accessor.count;
    const std::size_t   offset = view.byteOffset + accessor.byteOffset;
    const std::uint8_t* data = buffer.data.data() + offset;

    indices.reserve(count);

    switch(accessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        for(std::size_t i = 0; i < count; ++i)
            indices.push_back(static_cast<std::uint16_t>(data[i]));
        break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    {
        const std::uint16_t* ptr = reinterpret_cast<const std::uint16_t*>(data);
        for(std::size_t i = 0; i < count; ++i)
            indices.push_back(ptr[i]);
    }
    break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    {
        const std::uint32_t* ptr = reinterpret_cast<const std::uint32_t*>(data);
        for(std::size_t i = 0; i < count; ++i)
            indices.push_back(static_cast<std::uint16_t>(ptr[i]));
    }
    break;
    }

    return indices;
}

} // namespace

model::model(bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer)
    : m_vertex_buffer(vertex_buffer), m_index_buffer(index_buffer)
{
}

model model::load_from_file(std::string_view path)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model    gltf_model;
    std::string        error;
    std::string        warning;

    bool loaded = loader.LoadASCIIFromFile(&gltf_model, &error, &warning, std::string(path)) ||
                  loader.LoadBinaryFromFile(&gltf_model, &error, &warning, std::string(path));

    if(!warning.empty())
        std::clog << warning << std::endl;

    if(!loaded)
    {
        std::cerr << error << std::endl;
        return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
    }

    for(const auto& mesh: gltf_model.meshes)
    {
        for(const auto& primitive: mesh.primitives)
        {
            if(!primitive.attributes.contains("POSITION"))
                continue;

            auto vertices = extract_vertices(gltf_model, primitive);
            auto indices = extract_indices(gltf_model, primitive);

            auto                layout = make_vertex_layout();
            const bgfx::Memory* vertex_memory = bgfx::copy(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(vertex)));
            const bgfx::Memory* index_memory = bgfx::copy(indices.data(), static_cast<std::uint32_t>(indices.size() * sizeof(std::uint16_t)));

            auto vertex_buffer = bgfx::createVertexBuffer(vertex_memory, layout);
            auto index_buffer = bgfx::createIndexBuffer(index_memory);

            return model(vertex_buffer, index_buffer);
        }
    }

    return model(bgfx::VertexBufferHandle(bgfx::kInvalidHandle), bgfx::IndexBufferHandle(bgfx::kInvalidHandle));
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
