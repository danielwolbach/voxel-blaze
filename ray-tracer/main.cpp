#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <string>
#include <chrono>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <glm/gtc/type_ptr.hpp>

#define GL_CHECK(call)                              \
    do                                              \
    {                                               \
        call;                                       \
        GLenum err;                                 \
        while ((err = glGetError()) != GL_NO_ERROR) \
        {                                           \
            spdlog::error("OpenGL error  {}", err); \
        }                                           \
    } while (0)

const auto SCREEN_WIDTH = 1920U;
const auto SCREEN_HEIGHT = 1080U;
const auto ENABLE_VSYNC = false;
const auto OPENGL_MAJOR_VERSION = 4U;
const auto OPENGL_MINOR_VERSION = 6U;
const auto VOXEL_GRID_SIZE = glm::ivec3(256, 256, 256);

GLfloat QUAD_VERTICES[] =
    {
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
};

GLuint QUAD_INDICES[] =
    {
        0, 2, 1,
        0, 3, 2};

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

auto read_file(const std::string &filePath)
{
    std::ifstream input_stream(filePath);
    if (!input_stream.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::ostringstream content_stream;
    content_stream << input_stream.rdbuf();
    return content_stream.str();
}

int main()
{
    // Initialize environment.
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel Ray-Tracer", nullptr, nullptr);

    if (window == nullptr)
    {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(ENABLE_VSYNC);

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW.");
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Query size limits.
    {
        int max_texture_size = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

        int max_3d_texture_size = 0;
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_3d_texture_size);

        spdlog::info("max texture size: {}", max_texture_size);
        spdlog::info("max 3d texture size: {}", max_3d_texture_size);
        spdlog::info("voxel grid size: {} {} {} ({})", VOXEL_GRID_SIZE.x, VOXEL_GRID_SIZE.y, VOXEL_GRID_SIZE.z, VOXEL_GRID_SIZE.x * VOXEL_GRID_SIZE.y * VOXEL_GRID_SIZE.z);
    }

    // Set up vertex array.
    auto vertex_array = 0U;
    GL_CHECK(glCreateVertexArrays(1, &vertex_array));

    auto vertex_buffer = 0U;
    GL_CHECK(glCreateBuffers(1, &vertex_buffer));
    GL_CHECK(glNamedBufferData(vertex_buffer, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW));
    GL_CHECK(glEnableVertexArrayAttrib(vertex_array, 0));
    GL_CHECK(glVertexArrayAttribBinding(vertex_array, 0, 0));
    GL_CHECK(glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0));
    GL_CHECK(glEnableVertexArrayAttrib(vertex_array, 1));
    GL_CHECK(glVertexArrayAttribBinding(vertex_array, 1, 0));
    GL_CHECK(glVertexArrayAttribFormat(vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat)));
    GL_CHECK(glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, 5 * sizeof(GLfloat)));

    auto element_buffer = 0U;
    GL_CHECK(glCreateBuffers(1, &element_buffer));
    GL_CHECK(glNamedBufferData(element_buffer, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW));
    GL_CHECK(glVertexArrayElementBuffer(vertex_array, element_buffer));

    // Create vertex and fragment shaders.
    const auto shader_program = glCreateProgram();
    GL_CHECK();
    {
        const auto vertex_shader = compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
        const auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SOURCE);
        GL_CHECK(glAttachShader(shader_program, vertex_shader));
        GL_CHECK(glAttachShader(shader_program, fragment_shader));
        GL_CHECK(glLinkProgram(shader_program));
        GL_CHECK(glDeleteShader(vertex_shader));
        GL_CHECK(glDeleteShader(fragment_shader));
    }

    // Create compute shader.
    const auto tracer_program = glCreateProgram();
    GL_CHECK();
    {
        const auto compute_shader = compile_shader(GL_COMPUTE_SHADER, read_file("tracer.glsl").c_str());
        GL_CHECK(glAttachShader(tracer_program, compute_shader));
        GL_CHECK(glLinkProgram(tracer_program));
        GL_CHECK(glDeleteShader(compute_shader));
    }

    // Create screen texture.
    auto screen_texture = 0U;
    GL_CHECK(glCreateTextures(GL_TEXTURE_2D, 1, &screen_texture));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(screen_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureStorage2D(screen_texture, 1, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT));
    GL_CHECK(glBindImageTexture(0, screen_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));

    // Set timing variables.
    auto last_fps_time = std::chrono::high_resolution_clock::now();
    auto frame_count = 0U;
    auto fps = 0.0f;

    // Configure camera.
    auto camera_position = glm::vec3(0.0f, 0.0f, 0.0f);
    auto camera_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    auto camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
    auto camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    GL_CHECK(glUseProgram(tracer_program));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
    GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_right"), 1, glm::value_ptr(camera_right)));

    // Fill a voxel grid.
    std::vector<float> voxel_grid;
    voxel_grid.reserve(VOXEL_GRID_SIZE.x * VOXEL_GRID_SIZE.y * VOXEL_GRID_SIZE.z * 4);
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<GLfloat> distribution(0.0f, 1.0f);
    for (auto i = 0U; i < VOXEL_GRID_SIZE.x * VOXEL_GRID_SIZE.y * VOXEL_GRID_SIZE.z * 4; i += 4)
    {
        if (distribution(generator) > 0.95f)
        {
            voxel_grid.push_back(distribution(generator));
            voxel_grid.push_back(distribution(generator));
            voxel_grid.push_back(distribution(generator));
            voxel_grid.push_back(1.0f);
        }
        else
        {
            voxel_grid.push_back(0.0f);
            voxel_grid.push_back(0.0f);
            voxel_grid.push_back(0.0f);
            voxel_grid.push_back(0.0f);
        }
    }

    GLuint textureID;
    GL_CHECK(glGenTextures(1, &textureID));
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, textureID));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT));
    GL_CHECK(glTextureStorage3D(textureID, 1, GL_RGBA32F, VOXEL_GRID_SIZE.x, VOXEL_GRID_SIZE.y, VOXEL_GRID_SIZE.z));
    GL_CHECK(glBindImageTexture(1, textureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F));

    GL_CHECK(glBindTexture(GL_TEXTURE_3D, textureID));
    GL_CHECK(glTexSubImage3D(
        GL_TEXTURE_3D,
        0,
        0, 0, 0,
        VOXEL_GRID_SIZE.x,
        VOXEL_GRID_SIZE.y,
        VOXEL_GRID_SIZE.z,
        GL_RGBA,
        GL_FLOAT,
        voxel_grid.data()));

    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();

    // Render loop.
    while (!glfwWindowShouldClose(window))
    {

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentTime - previousTime;
        previousTime = currentTime;
        float deltaTime = duration.count();

        // Handle keyboard controls.
        float moveSpeed = deltaTime * 10.0f;
        float angle = deltaTime * 2.0f;

        if (glfwGetKey(window, GLFW_KEY_LEFT))
        {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
            camera_direction = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_direction, 1.0f)));
            camera_right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_right, 1.0f)));
            camera_up = glm::normalize(glm::cross(camera_right, camera_direction));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_right"), 1, glm::value_ptr(camera_right)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
            camera_direction = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_direction, 1.0f)));
            camera_right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_right, 1.0f)));
            camera_up = glm::normalize(glm::cross(camera_right, camera_direction));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_right"), 1, glm::value_ptr(camera_right)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
        }

        if (glfwGetKey(window, GLFW_KEY_UP))
        {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, camera_right);
            camera_direction = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_direction, 1.0f)));
            camera_up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_up, 1.0f)));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN))
        {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -angle, camera_right);
            camera_direction = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_direction, 1.0f)));
            camera_up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera_up, 1.0f)));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_direction"), 1, glm::value_ptr(camera_direction)));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_up"), 1, glm::value_ptr(camera_up)));
        }

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            camera_position += moveSpeed * glm::normalize(camera_direction * glm::vec3(1, 0, 1));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        if (glfwGetKey(window, GLFW_KEY_S))
        {
            camera_position -= moveSpeed * glm::normalize(camera_direction * glm::vec3(1, 0, 1));
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            camera_position += moveSpeed * camera_right;
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        if (glfwGetKey(window, GLFW_KEY_A))
        {
            camera_position -= moveSpeed * camera_right;
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE))
        {
            camera_position += moveSpeed * glm::vec3(0, 1, 0);
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
            camera_position -= moveSpeed * glm::vec3(0, 1, 0);
            GL_CHECK(glUseProgram(tracer_program));
            GL_CHECK(glUniform3fv(glGetUniformLocation(tracer_program, "camera_position"), 1, glm::value_ptr(camera_position)));
        }

        GL_CHECK(glUseProgram(tracer_program));
        GL_CHECK(glDispatchCompute(ceil(SCREEN_WIDTH / 8), ceil(SCREEN_HEIGHT / 4), 1));
        GL_CHECK(glMemoryBarrier(GL_ALL_BARRIER_BITS));

        GL_CHECK(glUseProgram(shader_program));
        GL_CHECK(glBindTextureUnit(0, screen_texture));
        GL_CHECK(glBindVertexArray(vertex_array));
        GL_CHECK(glUniform1i(glGetUniformLocation(shader_program, "screen"), 0));
        GL_CHECK(glDrawElements(GL_TRIANGLES, sizeof(QUAD_INDICES) / sizeof(QUAD_INDICES[0]), GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Count frames per second.
        frame_count += 1;
        const auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = current_time - last_fps_time;
        if (elapsed.count() >= 1.0f)
        {
            fps = frame_count / elapsed.count();
            std::cout << "FPS: " << fps << std::endl;
            last_fps_time = current_time;
            frame_count = 0;

            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("pos   {} {} {} {}", camera_position.x, camera_position.y, camera_position.z, glm::length(camera_position));
            spdlog::debug("dir   {} {} {} {}", camera_direction.x, camera_direction.y, camera_direction.z, glm::length(camera_direction));
            spdlog::debug("upv   {} {} {} {}", camera_up.x, camera_up.y, camera_up.z, glm::length(camera_up));
            spdlog::debug("rig   {} {} {} {}", camera_right.x, camera_right.y, camera_right.z, glm::length(camera_right));
        }
    }

    // Terminate everything.
    GL_CHECK(glDeleteProgram(shader_program));
    GL_CHECK(glDeleteVertexArrays(1, &vertex_array));
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}