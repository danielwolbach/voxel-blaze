#pragma once

#include <voxel_blaze/core/types.hpp>
#include <voxel_blaze/graphics/index_buffer.hpp>
#include <voxel_blaze/graphics/vertex_buffer.hpp>

namespace voxel_blaze
{
    class VertexArray
    {
      public:
        VertexArray(const Shared<IndexBuffer> &index_buffer);

        ~VertexArray();

        void bind() const;

        void add_vertex_buffer(const Shared<VertexBuffer> &vertex_buffer) const;

        u32 element_count() const;

      private:
        u32 handle = 0;
        const Shared<IndexBuffer> index_buffer;
    };
}
