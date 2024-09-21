#include <chrono>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/rasterizer.hpp>
#include <voxel-blaze/graphics/ray_tracer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>
#include <voxel-blaze/test.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>

void test_suite()
{
    const auto test_duration = 5.0f;
    const auto tests = {
        // Magica-Voxel-Modelle
        // Test::from_vox_path("resources/teapot.vox"),
        // Test::from_vox_path("resources/monu.vox"),
        // Test::from_vox_path("resources/scene_coffee.vox"),
        // Test::from_vox_path("resources/scene_park.vox"),

        // Frequenz 0.05
        Test::from_perlin_noise(0.05f, 16),
        Test::from_perlin_noise(0.05f, 32),
        Test::from_perlin_noise(0.05f, 64),
        Test::from_perlin_noise(0.05f, 128),
        Test::from_perlin_noise(0.05f, 256),
        Test::from_perlin_noise(0.05f, 512),

        // Frequenz 0.1
        Test::from_perlin_noise(0.1f, 16),
        Test::from_perlin_noise(0.1f, 32),
        Test::from_perlin_noise(0.1f, 64),
        Test::from_perlin_noise(0.1f, 128),
        Test::from_perlin_noise(0.1f, 256),
        Test::from_perlin_noise(0.1f, 512),
    };

    std::vector<Test::Result> results;

    const auto resolutions = {
        glm::ivec2(1280, 720),
        glm::ivec2(1920, 1080),
    };

    for (const auto &resolution : resolutions)
    {
        for (const auto &test : tests)
        {
            results.push_back(test.run(test_duration, Test::RenderMode::Rasterize, resolution));
        }

        for (const auto &test : tests)
        {
            results.push_back(test.run(test_duration, Test::RenderMode::RayTrace, resolution));
        }
    }

    for (const auto &result : results)
    {
        std::cout << result.to_string() << "\n";
    }

    exit(0);
}

int main()
{
    // test_suite();

    // const auto voxel_grid = std::make_unique<ArrayVoxelGrid>(64, 64, 64);
    // voxel_grid->fill_perlin_noise(0.05);
    const auto window = Window(1280, 1280);
    const auto parser = VoxParser("resources/teapot.vox");
    const auto voxel_grid = parser.get_voxel_grid();
    const auto renderer = RayTracer(*voxel_grid, window);
    auto camera = OrbitCamera(
        voxel_grid->max_size() * 2,
        glm::vec3(voxel_grid->get_size_x() / 2.0, voxel_grid->get_size_y() / 2.0, voxel_grid->get_size_z() / 2.0));

    camera.set(voxel_grid->max_size() * 2, glm::radians(60.0f), glm::radians(30.0f));

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
            camera.move(0.0f, -delta, 0.0f);
        }
        if (window.key_down(GLFW_KEY_DOWN))
        {
            camera.move(0.0f, delta, 0.0f);
        }
        if (window.key_down(GLFW_KEY_LEFT))
        {
            camera.move(0.0f, 0.0f, delta);
        }
        if (window.key_down(GLFW_KEY_RIGHT))
        {
            camera.move(0.0f, 0.0f, -delta);
        }
        if (window.key_down(GLFW_KEY_O))
        {
            camera.move(-delta * 10.0f, 0.0f, 0.0f);
        }
        if (window.key_down(GLFW_KEY_P))
        {
            camera.move(delta * 10.0f, 0.0f, 0.0f);
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
