#include <chrono>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/rasterizer.hpp>
#include <voxel-blaze/graphics/ray_tracer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>

int main()
{
    const auto window = Window(1280, 720);
    const auto parser = VoxParser("resources/scene_orgy.vox");
    const auto voxel_grid = parser.get_voxel_grid();
    const auto renderer = RayTracer(*voxel_grid);
    auto camera = OrbitCamera(voxel_grid->max_size() * 1.5, glm::vec3(voxel_grid->get_size_x() / 2.0, voxel_grid->get_size_y() / 2.0, voxel_grid->get_size_z() / 2.0));

    // Configure timing variables.
    auto last_fps_time = std::chrono::high_resolution_clock::now();
    auto frame_count = 0U;
    auto fps = 0.0f;
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();

    while (window.opened())
    {
        // Reset timing variables.
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentTime - previousTime;
        previousTime = currentTime;
        float delta = duration.count();

        // Render the voxel grid.
        renderer.render(camera);

        // Control the orbit camera.
        if (window.key_down(GLFW_KEY_UP))
        {
            camera.move(-delta, 0.0f);
        }
        if (window.key_down(GLFW_KEY_DOWN))
        {
            camera.move(delta, 0.0f);
        }
        if (window.key_down(GLFW_KEY_LEFT))
        {
            camera.move(0.0f, delta);
        }
        if (window.key_down(GLFW_KEY_RIGHT))
        {
            camera.move(0.0f, -delta);
        }

        // Count frames per second.
        frame_count += 1;
        const auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = current_time - last_fps_time;
        if (elapsed.count() >= 1.0f)
        {
            fps = frame_count / elapsed.count();
            fps /= 2.0f;
            spdlog::info("FPS: {}", static_cast<unsigned>(fps));
            last_fps_time = current_time;
            frame_count = 0;
        }
    }
}
