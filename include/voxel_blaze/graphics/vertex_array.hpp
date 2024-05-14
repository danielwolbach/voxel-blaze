#pragma once

#include <voxel_blaze/core/types.hpp>
#include <voxel_blaze/graphics/index_buffer.hpp>
#include <voxel_blaze/graphics/vertex_buffer.hpp>

namespace voxel_blaze
{
    class VertexArray
    {
      public:
        VertexArray(IndexBuffer &&index_buffer);

        ~VertexArray();

        void bind() const;

        void add_vertex_buffer(VertexBuffer &&vertex_buffer);

        u32 element_count() const;

      private:
        u32 handle = 0;
        IndexBuffer index_buffer;
        Vec<VertexBuffer> vertex_buffers;
    };
}
