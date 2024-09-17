#include <voxel-blaze/graphics/ray_tracer.hpp>

GLfloat QUAD_VERTICES[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
};

GLuint QUAD_INDICES[] = {0, 2, 1, 0, 3, 2};

const char *VERTEX_SHADER_SOURCE = R"glsl(
#version 460 core
layout (location = 0) in vec3 position_coordinates;
layout (location = 1) in vec2 texture_coordinates_in;
out vec2 texture_coordinates;
void main()
{
    texture_coordinates = texture_coordinates_in;
    gl_Position = vec4(position_coordinates.xyz, 1.0);
}
)glsl";

const char *FRAGMENT_SHADER_SOURCE = R"glsl(
#version 460 core
out vec4 output_color;
in vec2 texture_coordinates;
uniform sampler2D screen;
void main()
{
    output_color = texture(screen, texture_coordinates);
}
)glsl";

auto compile_shader(GLenum type, const char *source)
{
    const auto shader = glCreateShader(type);
    GL_CHECK(glShaderSource(shader, 1, &source, nullptr));
    GL_CHECK(glCompileShader(shader));

    GLint success = 0;
    GLchar log[512];
    GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GL_CHECK(glGetShaderInfoLog(shader, 512, nullptr, log));
        spdlog::error("Failed to compile shader: {}", log);
    }

    return shader;
}

auto read_file(const std::string &file_path)
{
    std::ifstream input_stream(file_path);
    if (!input_stream.is_open())
    {
        throw std::runtime_error("Could not open file: " + file_path);
    }
    std::ostringstream content_stream;
    content_stream << input_stream.rdbuf();
    return content_stream.str();
}

RayTracer::RayTracer(const VoxelGrid &voxel_grid)
{
    // Set up vertex array.
    vertex_array = 0U;
    GL_CHECK(glCreateVertexArrays(1, &vertex_array));

    vertex_buffer = 0U;
    GL_CHECK(glCreateBuffers(1, &vertex_buffer));
    GL_CHECK(glNamedBufferData(vertex_buffer, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW));
    GL_CHECK(glEnableVertexArrayAttrib(vertex_array, 0));
    GL_CHECK(glVertexArrayAttribBinding(vertex_array, 0, 0));
    GL_CHECK(glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0));
    GL_CHECK(glEnableVertexArrayAttrib(vertex_array, 1));
    GL_CHECK(glVertexArrayAttribBinding(vertex_array, 1, 0));
    GL_CHECK(glVertexArrayAttribFormat(vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat)));
    GL_CHECK(glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, 5 * sizeof(GLfloat)));

    element_buffer = 0U;
    GL_CHECK(glCreateBuffers(1, &element_buffer));
    GL_CHECK(glNamedBufferData(element_buffer, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW));
    GL_CHECK(glVertexArrayElementBuffer(vertex_array, element_buffer));

    // Create vertex and fragment shaders.
    screen_program = glCreateProgram();
    {
        const auto vertex_shader = compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
        const auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SOURCE);
        GL_CHECK(glAttachShader(screen_program, vertex_shader));
        GL_CHECK(glAttachShader(screen_program, fragment_shader));
        GL_CHECK(glLinkProgram(screen_program));
        GL_CHECK(glDeleteShader(vertex_shader));
        GL_CHECK(glDeleteShader(fragment_shader));
    }

    // Create compute shader.
    tracer_program = glCreateProgram();
    {
        const auto compute_shader = compile_shader(GL_COMPUTE_SHADER, read_file("source/graphics/tracer.comp").c_str());
        GL_CHECK(glAttachShader(tracer_program, compute_shader));
        GL_CHECK(glLinkProgram(tracer_program));
        GL_CHECK(glDeleteShader(compute_shader));
    }

    // Create screen texture.
    screen_texture = 0U;
    GL_CHECK(glCreateTextures(GL_TEXTURE_2D, 1, &screen_texture));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureStorage2D(screen_texture, 1, GL_RGBA32F, 1280, 720)); // fixme
    GL_CHECK(glBindImageTexture(0, screen_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));

    //Fill a voxel grid.
    glm::vec3 voxel_grid_size(voxel_grid.get_size_x(), voxel_grid.get_size_y(), voxel_grid.get_size_z());

    const auto data = voxel_grid.raw_values();

    // std::vector<float> data;
    // data.reserve(voxel_grid_size.x * voxel_grid_size.y * voxel_grid_size.z * 4);

    // for (auto i = 0; i < voxel_grid_size.x * voxel_grid_size.y * voxel_grid_size.z * 4; i += 4)
    // {
    //     data.push_back(0.5f);
    //     data.push_back(0.5f);
    //     data.push_back(0.5f);
    //     data.push_back(1.0f);
    // }

    // Create 3D texture
    voxel_grid_texture = 0U;
    assert(data.size() == voxel_grid_size.x * voxel_grid_size.y * voxel_grid_size.z * 4);
    GL_CHECK(glGenTextures(1, &voxel_grid_texture));
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, voxel_grid_texture));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT));
    GL_CHECK(glTextureStorage3D(voxel_grid_texture, 1, GL_RGBA32F, voxel_grid_size.x, voxel_grid_size.y, voxel_grid_size.z));
    GL_CHECK(glBindImageTexture(1, voxel_grid_texture, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F));
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, voxel_grid_texture));
    GL_CHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, voxel_grid_size.x, voxel_grid_size.y, voxel_grid_size.z,
                             GL_RGBA, GL_FLOAT, data.data()));
}

void RayTracer::render(const OrbitCamera &camera) const
{
    GL_CHECK(glUseProgram(tracer_program));

    const auto camera_position = camera.get_position();
    const auto camera_direction = glm::normalize(camera.get_direction());
    const auto camera_right = glm::normalize(glm::cross(camera.get_up(), camera_direction));
    const auto camera_up = glm::normalize(glm::cross(camera_right, camera_direction));

    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_right"), 1, glm::value_ptr(camera_right)));
    

    // auto camera_position = glm::vec3(0.0f, 0.0f, 200.0f);
    // auto camera_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    // auto camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
    // auto camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    // GL_CHECK(glUseProgram(tracer_program));
    // GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
    // GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
    // GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
    // GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_right"), 1, glm::value_ptr(camera_right)));

    GL_CHECK(glDispatchCompute(ceil(1280 / 8), ceil(720 / 4), 1));
    GL_CHECK(glMemoryBarrier(GL_ALL_BARRIER_BITS));

    GL_CHECK(glUseProgram(screen_program));
    GL_CHECK(glBindTextureUnit(0, screen_texture));
    GL_CHECK(glBindVertexArray(vertex_array));
    GL_CHECK(glUniform1i(glGetUniformLocation(screen_program, "screen"), 0));
    GL_CHECK(glDrawElements(GL_TRIANGLES, sizeof(QUAD_INDICES) / sizeof(QUAD_INDICES[0]), GL_UNSIGNED_INT, 0));
}