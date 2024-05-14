#pragma once

#include <voxel_blaze/core/types.hpp>

namespace voxel_blaze
{
    /// Clear all queried OpenGL errors.
    void clear_gl_errors();

    /// Throw an OpenGL error if one is queried.
    void throw_gl_errors();
}

#define GL_CHECK(...)                 \
    ::voxel_blaze::clear_gl_errors(); \
    __VA_ARGS__;                      \
    ::voxel_blaze::throw_gl_errors();
