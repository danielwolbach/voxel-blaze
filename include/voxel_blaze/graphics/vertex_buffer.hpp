#pragma once

#include <voxel_blaze/core/types.hpp>

namespace voxel_blaze
{
    class VertexBuffer
    {
      public:
        explicit VertexBuffer(const Vec<f32> &data);

        VertexBuffer(VertexBuffer &&other);

        ~VertexBuffer();

        void bind() const;

        VertexBuffer(const VertexBuffer &other) = delete;
        VertexBuffer &operator=(const VertexBuffer &other) = delete;

      private:
        u32 handle = 0;
    };
}
