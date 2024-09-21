
#include <voxel-blaze/test.hpp>

Test::Test(const std::string &name, std::unique_ptr<VoxelGrid> &&voxel_grid)
    : name(name), voxel_grid(std::move(voxel_grid))
{
}

Test Test::from_vox_path(const std::string &path)
{
    const VoxParser parser(path);
    auto voxel_grid = parser.get_voxel_grid();
    return Test(path, std::move(voxel_grid));
}

Test Test::from_perlin_noise(const float frequency, const unsigned size)
{
    auto voxel_grid = std::make_unique<ArrayVoxelGrid>(size, size, size);
    voxel_grid->fill_perlin_noise(frequency);
    return Test("perlin noise", std::move(voxel_grid));
}

Test::Result Test::run(const float test_duration, const RenderMode render_mode, const glm::ivec2 res) const
{
    // Set up everything.
    std::vector<float> fps_list;
    const auto window = Window(res.x, res.y);
    const auto camera_radius = voxel_grid->max_size() * 1.5;
    auto camera = OrbitCamera(camera_radius, glm::vec3(voxel_grid->get_size_x() / 2.0, voxel_grid->get_size_y() / 2.0,
                                                       voxel_grid->get_size_z() / 2.0));
    std::unique_ptr<Renderer> renderer;
    if (render_mode == Test::RenderMode::Rasterize)
    {
        renderer = std::make_unique<Rasterizer>(*voxel_grid, window);
    }
    else if (render_mode == Test::RenderMode::RayTrace)
    {
        renderer = std::make_unique<RayTracer>(*voxel_grid, window);
    }

    // Configure timing variables.
    auto last_fps_time = std::chrono::high_resolution_clock::now();
    auto frame_count = 0U;
    auto fps = 0.0f;
    auto previous_time = std::chrono::high_resolution_clock::now();
    const auto start_time = std::chrono::high_resolution_clock::now();

    while (window.opened() || true)
    {
        // Reset timing variables.
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentTime - previous_time;
        previous_time = currentTime;
        float delta = duration.count();

        // Render the voxel grid.
        renderer->render(camera);

        // Count frames per second.
        frame_count += 1;
        const auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed_since_last = current_time - last_fps_time;
        if (elapsed_since_last.count() >= 1.0f)
        {
            fps = frame_count / elapsed_since_last.count();
            fps /= 2.0f;
            fps_list.push_back(fps);
            spdlog::info("FPS: {}", static_cast<unsigned>(fps));
            last_fps_time = current_time;
            frame_count = 0;
        }

        // Count total running duration
        std::chrono::duration<float> elapsed_since_start = current_time - start_time;
        if (elapsed_since_start.count() >= test_duration)
        {
            break;
        }

        camera.set(camera_radius, glm::sin(elapsed_since_start.count()) * 0.5f + glm::radians(90.0f),
                   elapsed_since_start.count());
    }

    auto average_fps = 0.0f;

    for (const auto fps_value : fps_list)
    {
        average_fps += fps_value;
    }

    average_fps /= fps_list.size();

    return Result{render_mode, name, voxel_grid->get_size(), res, voxel_grid->count_voxels(), average_fps};
}