#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/utils.hpp>
#include <voxel_blaze/graphics/vertex_array.hpp>

namespace voxel_blaze
{
    VertexArray::VertexArray(const Shared<IndexBuffer> &index_buffer) : index_buffer(index_buffer)
    {
        GL_CHECK(glGenVertexArrays(1, &handle));
        GL_CHECK(glBindVertexArray(handle));

        index_buffer->bind();

        GL_CHECK(glBindVertexArray(0));
    }

    VertexArray::~VertexArray()
    {
        GL_CHECK(glDeleteVertexArrays(1, &handle));
    }

    void VertexArray::bind() const
    {
        GL_CHECK(glBindVertexArray(handle));
    }

    void VertexArray::add_vertex_buffer(const Shared<VertexBuffer> &vertex_buffer) const
    {
        bind();
        vertex_buffer->bind();

        // TODO Don't hard-code the vertex buffer layout.
        GL_CHECK(glEnableVertexAttribArray(0));
        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr));
        GL_CHECK(glDisableVertexAttribArray(0));
    }

    u32 VertexArray::element_count() const
    {
        return index_buffer->size();
    }
}
