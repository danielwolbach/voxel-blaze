#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/shader.hpp>
#include <voxel_blaze/graphics/utils.hpp>

namespace voxel_blaze
{
    Shader::Shader(const String &vertex_source, const String &fragment_source)
    {
        u32 vertexShaderHandle = compile(vertex_source, Shader::ModuleType::Vertex);
        u32 fragmentShaderHandle = compile(fragment_source, Shader::ModuleType::Fragment);

        GL_CHECK(handle = glCreateProgram());

        GL_CHECK(glAttachShader(handle, vertexShaderHandle));
        GL_CHECK(glAttachShader(handle, fragmentShaderHandle));
        GL_CHECK(glLinkProgram(handle));
        GL_CHECK(glValidateProgram(handle));
        GL_CHECK(glDetachShader(handle, vertexShaderHandle));
        GL_CHECK(glDetachShader(handle, fragmentShaderHandle));
        GL_CHECK(glDeleteShader(vertexShaderHandle));
        GL_CHECK(glDeleteShader(fragmentShaderHandle));
    }

    Shader::~Shader()
    {
        GL_CHECK(glDeleteProgram(handle));
    }

    void Shader::use() const
    {
        GL_CHECK(glUseProgram(handle));
    }

    u32 Shader::compile(const String &source, const Shader::ModuleType module_type) const
    {
        u32 module_handle = 0;

        if (module_type == Shader::ModuleType::Vertex)
        {
            GL_CHECK(module_handle = glCreateShader(GL_VERTEX_SHADER));
        }
        else if (module_type == Shader::ModuleType::Fragment)
        {
            GL_CHECK(module_handle = glCreateShader(GL_FRAGMENT_SHADER));
        }

        const char *c_source = source.c_str();

        GL_CHECK(glShaderSource(module_handle, 1, &c_source, nullptr));
        GL_CHECK(glCompileShader(module_handle));

        int success = 0;
        GL_CHECK(glGetShaderiv(module_handle, GL_COMPILE_STATUS, &success));

        if (success == false)
        {
            GLint message_length = 255;
            char *message = new char[message_length];

            GL_CHECK(glGetShaderiv(module_handle, GL_INFO_LOG_LENGTH, &message_length));
            GL_CHECK(glGetShaderInfoLog(module_handle, message_length, &message_length, message));

            throw RuntimeError(fmt::format("Failed to compile shader module: {}", message));

            delete[] message;
            GL_CHECK(glDeleteShader(module_handle));
        }

        return module_handle;
    }
}
