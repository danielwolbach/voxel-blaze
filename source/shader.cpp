#include <voxel-blaze/shader.hpp>

Shader::Shader(const std::string &vertex_source, const std::string &fragment_source)
{
    const unsigned vertex_module = compile(vertex_source, GL_VERTEX_SHADER);
    const unsigned fragment_module = compile(fragment_source, GL_FRAGMENT_SHADER);

    handle = glCreateProgram();

    glAttachShader(handle, vertex_module);
    glAttachShader(handle, fragment_module);
    glLinkProgram(handle);
    glValidateProgram(handle);
    glDeleteShader(vertex_module);
    glDeleteShader(fragment_module);
}

Shader::~Shader()
{
    glDeleteProgram(handle);
}

unsigned Shader::compile(const std::string &source, const unsigned type) const
{
    const char *source_ptr = source.c_str();
    const unsigned shader_module = glCreateShader(type);
    glShaderSource(shader_module, 1, &source_ptr, nullptr);
    glCompileShader(shader_module);

    int success = 0;
    glGetShaderiv(shader_module, GL_COMPILE_STATUS, &success);

    // TODO This needs improvement.
    if (success == false)
    {
        int message_length = 255;
        char *message_arr = new char[message_length];
        glGetShaderiv(shader_module, GL_INFO_LOG_LENGTH, &message_length);
        glGetShaderInfoLog(shader_module, message_length, &message_length, message_arr);
        std::string message = message_arr;
        delete[] message_arr;
        spdlog::error("Failed to compile shader: {}", message);
        glDeleteShader(shader_module);
    }

    return shader_module;
}

void Shader::draw(const Model &model) const
{
    glUseProgram(handle);
    glBindVertexArray(model.vertex_array);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Shader::upload_transform(const std::string &location, const float *data) const
{
    glUseProgram(handle);
    const int location_identifier = glGetUniformLocation(handle, location.c_str());

    if (location_identifier >= 0)
    {
        glUniformMatrix4fv(location_identifier, 1, GL_FALSE, data);
    }
    else
    {
        spdlog::error("Failed to upload transform to location `{}`", location);
    }

    glUseProgram(0);
}
