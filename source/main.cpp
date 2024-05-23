#include <voxel-blaze/model.hpp>
#include <voxel-blaze/shader.hpp>
#include <voxel-blaze/voxel.hpp>
#include <voxel-blaze/window.hpp>

const char *vertex_shader_source = R""""(
#version 440 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
out vec3 pass_color;
uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main()
{
	pass_color = in_color;
	gl_Position = projection_transform * view_transform * model_transform * vec4(in_position, 1.0);
}
)"""";

const char *fragment_shader_source = R""""(
#version 440 core

in vec3 pass_color;
out vec4 out_color;

void main()
{
	out_color = vec4(pass_color, 1.0f);
}
)"""";

const std::vector<float> cube_vertices = {
    // Front face
    -0.5f, -0.5f, 0.5f, // Position
    1.0f, 0.0f, 0.0f,   // Color
    0.5f, -0.5f, 0.5f,  // Position
    1.0f, 0.0f, 0.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    1.0f, 0.0f, 0.0f,   // Color
    -0.5f, 0.5f, 0.5f,  // Position
    1.0f, 0.0f, 0.0f,   // Color

    // Back face
    -0.5f, -0.5f, -0.5f, // Position
    0.0f, 1.0f, 0.0f,    // Color
    0.5f, -0.5f, -0.5f,  // Position
    0.0f, 1.0f, 0.0f,    // Color
    0.5f, 0.5f, -0.5f,   // Position
    0.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, -0.5f,  // Position
    0.0f, 1.0f, 0.0f,    // Color

    // Right face
    0.5f, -0.5f, 0.5f,  // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, -0.5f, -0.5f, // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, 0.5f, -0.5f,  // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    0.0f, 0.0f, 1.0f,   // Color

    // Left face
    -0.5f, -0.5f, 0.5f,  // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, -0.5f, -0.5f, // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, -0.5f,  // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, 0.5f,   // Position
    1.0f, 1.0f, 0.0f,    // Color

    // Top face
    -0.5f, 0.5f, 0.5f,  // Position
    0.0f, 1.0f, 1.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    0.0f, 1.0f, 1.0f,   // Color
    0.5f, 0.5f, -0.5f,  // Position
    0.0f, 1.0f, 1.0f,   // Color
    -0.5f, 0.5f, -0.5f, // Position
    0.0f, 1.0f, 1.0f,   // Color

    // Bottom face
    -0.5f, -0.5f, 0.5f,  // Position
    1.0f, 0.0f, 1.0f,    // Color
    0.5f, -0.5f, 0.5f,   // Position
    1.0f, 0.0f, 1.0f,    // Color
    0.5f, -0.5f, -0.5f,  // Position
    1.0f, 0.0f, 1.0f,    // Color
    -0.5f, -0.5f, -0.5f, // Position
    1.0f, 0.0f, 1.0f     // Color
};

const std::vector<unsigned> cube_indices = {
    0,  1,  2,  2,  3,  0,  // Front face
    4,  5,  6,  6,  7,  4,  // Back face
    8,  9,  10, 10, 11, 8,  // Right face
    12, 13, 14, 14, 15, 12, // Left face
    16, 17, 18, 18, 19, 16, // Top face
    20, 21, 22, 22, 23, 20  // Bottom face
};

int main()
{
    Window window(1280, 720);
    Shader shader(vertex_shader_source, fragment_shader_source);
    // Model model(cube_indices, cube_vertices);
    Chunk chunk(10);
    chunk = Chunk::spherical(32);
    Model model = chunk.meshify_naive();

    glm::mat4 model_transform = glm::mat4(1.0f);
    shader.upload_transform("model_transform", glm::value_ptr(model_transform));

    glm::mat4 projection_transform = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
    shader.upload_transform("projection_transform", glm::value_ptr(projection_transform));

    float distance = 5.0f;
    glm::vec3 camera_position(0.0f, 0.0f, distance);
    glm::vec3 target_position = chunk.center();

    while (window.opened())
    {
        shader.draw(model);

        glm::mat4 view_transform = glm::lookAt(camera_position, target_position, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.upload_transform("view_transform", glm::value_ptr(view_transform));

        {
            float speed = glm::distance(camera_position, target_position) * 0.005f;
            glm::vec3 front = glm::normalize(target_position - camera_position);
            glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
            glm::vec3 up = glm::normalize(glm::cross(right, front));

            if (window.key_down(GLFW_KEY_UP))
            {
                camera_position += speed * up;
            }
            if (window.key_down(GLFW_KEY_DOWN))
            {
                camera_position -= speed * up;
            }
            if (window.key_down(GLFW_KEY_LEFT))
            {
                camera_position -= speed * right;
            }
            if (window.key_down(GLFW_KEY_RIGHT))
            {
                camera_position += speed * right;
            }
            if (window.key_down(GLFW_KEY_PAGE_UP))
            {
                camera_position += speed * front;
            }
            if (window.key_down(GLFW_KEY_PAGE_DOWN))
            {
                camera_position -= speed * front;
            }
        }
    }
}
