#include "spdlog/common.h"
#include "spdlog/spdlog.h"
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

int test()
{
    for (unsigned i = 1; i <= 64; i *= 2)
    {
        Chunk chunk = Chunk::filled(i);
        Model model = chunk.meshify_naive();
    }

    exit(1);
}

int main()
{
    spdlog::set_level(spdlog::level::info);
    Window window(720, 720);
    test();
    auto epsilon = glm::epsilon<float>();
    auto size = 8;
    Shader shader(vertex_shader_source, fragment_shader_source);
    Chunk chunk = Chunk::spherical(size);
    Model model = chunk.meshify_naive();

    glm::mat4 model_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-size / 2, -size / 2, -size / 2));
    // glm::mat4 model_transform = glm::mat4(1.0f);
    shader.upload_transform("model_transform", glm::value_ptr(model_transform));

    glm::mat4 projection_transform = glm::perspective(glm::radians(45.0f), 720.0f / 720.0f, 0.1f, 10000.0f);
    shader.upload_transform("projection_transform", glm::value_ptr(projection_transform));

    bool wire_frame = false;

    float distance = (float)size * 2.5f;
    float theta = glm::radians(45.0);
    float phi = glm::radians(70.0);

    glm::vec3 target_position(0.0f, 0.0f, 0.0f);

    // CLOCK
    auto last_fps_time = std::chrono::high_resolution_clock::now();
    auto last_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    double fps = 0.0;

    while (window.opened())
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(start_time - last_time).count();

        {
            phi = glm::clamp(phi, epsilon, glm::pi<float>() - epsilon);
            const auto x = distance * glm::sin(phi) * glm::cos(theta);
            const auto y = distance * glm::cos(phi);
            const auto z = distance * glm::sin(phi) * glm::sin(theta);
            const auto view_transform = glm::lookAt(glm::vec3(x, y, z), target_position, glm::vec3(0.0f, 1.0f, 0.0f));
            shader.upload_transform("view_transform", glm::value_ptr(view_transform));

            if (window.key_down(GLFW_KEY_UP))
            {
                phi -= delta_time;
            }
            if (window.key_down(GLFW_KEY_DOWN))
            {
                phi += delta_time;
            }
            if (window.key_down(GLFW_KEY_LEFT))
            {
                theta += delta_time;
            }
            if (window.key_down(GLFW_KEY_RIGHT))
            {
                theta -= delta_time;
            }
            if (window.key_down(GLFW_KEY_PAGE_UP))
            {
                distance -= delta_time * 100.0f;
            }
            if (window.key_down(GLFW_KEY_PAGE_DOWN))
            {
                distance += delta_time * 100.0f;
            }

            if (window.key_down(GLFW_KEY_SPACE))
            {
                wire_frame = !wire_frame;
                if (wire_frame)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else
                {
                    glLineWidth(2.0f);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            }
        }

        shader.draw(model);

        // Render loop end
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frame_duration = end_time - start_time;
        double frame_time = frame_duration.count();

        frame_count++;
        last_time = end_time;

        // Calculate FPS every second
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - last_fps_time;
        if (elapsed.count() >= 1.0)
        {
            fps = frame_count / elapsed.count();
            last_fps_time = now;
            frame_count = 0;

            // Use spdlog for logging
            spdlog::info("FPS: {:.2f}, Frame Time: {:.6f} ms", fps, frame_time * 1000.0);
        }
    }
}
