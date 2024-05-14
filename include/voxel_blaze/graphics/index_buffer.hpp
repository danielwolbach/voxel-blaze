#pragma once

#include <voxel_blaze/core/types.hpp>

namespace voxel_blaze
{
    class IndexBuffer
    {
      public:
        explicit IndexBuffer(const Vec<u32> &indices);

        ~IndexBuffer();

        void bind() const;

        u32 size() const;

        IndexBuffer(const IndexBuffer &other) = delete;
        IndexBuffer &operator=(const IndexBuffer &other) = delete;

      private:
        u32 handle = 0;
        u32 vertex_count;
    };
}
