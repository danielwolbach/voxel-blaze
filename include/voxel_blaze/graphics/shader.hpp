#pragma once

#include <voxel_blaze/core/types.hpp>

namespace voxel_blaze
{
    class Shader
    {
      public:
        Shader(const String &vertex_source, const String &fragment_source);

        ~Shader();

        void use() const;

        Shader(const Shader &other) = delete;
        Shader &operator=(const Shader &other) = delete;

      private:
        enum class ModuleType
        {
            Vertex,
            Fragment
        };

        u32 compile(const String &source, const ModuleType module_type) const;

        u32 handle = 0;
    };
}
