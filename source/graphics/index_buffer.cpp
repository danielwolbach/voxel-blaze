#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/index_buffer.hpp>
#include <voxel_blaze/graphics/utils.hpp>

namespace voxel_blaze
{
    IndexBuffer::IndexBuffer(const Vec<u32> &indices) : vertex_count(indices.size())
    {
        GL_CHECK(glGenBuffers(1, &handle));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), &indices[0], GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    IndexBuffer::~IndexBuffer()
    {
        GL_CHECK(glDeleteBuffers(1, &handle));
    }

    void IndexBuffer::bind() const
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
    }

    u32 IndexBuffer::size() const
    {
        return vertex_count;
    }
}
