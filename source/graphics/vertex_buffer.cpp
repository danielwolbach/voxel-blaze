#include <fmt/core.h>
#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/utils.hpp>
#include <voxel_blaze/graphics/vertex_buffer.hpp>

namespace voxel_blaze
{
    VertexBuffer::VertexBuffer(const Vec<f32> &data)
    {
        GL_CHECK(glGenBuffers(1, &handle));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(f32), &data[0], GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    VertexBuffer::VertexBuffer(VertexBuffer &&other) : handle(other.handle)
    {
        fmt::println("Move vertex buffer {} from object {} to {}", handle, fmt::ptr(&other), fmt::ptr(this));
        other.handle = 0;
    }

    VertexBuffer::~VertexBuffer()
    {
        fmt::println("Delete vertex buffer {} of object {}", handle, fmt::ptr(this));
        GL_CHECK(glDeleteBuffers(1, &handle));
    }

    void VertexBuffer::bind() const
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle));
    }
}
