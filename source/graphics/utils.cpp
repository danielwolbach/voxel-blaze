#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/utils.hpp>

namespace voxel_blaze
{
    void clear_gl_errors()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    void throw_gl_errors()
    {
        if (i32 error_code = glGetError())
        {
            // TODO Improve error handling.
            const unsigned char *error_string = glewGetErrorString(error_code);
            String error_description = reinterpret_cast<const char *>(error_string);
            throw RuntimeError(fmt::format("An OpenGL error has occurred: {} ({})", error_description, error_code));
        }
    }
}
