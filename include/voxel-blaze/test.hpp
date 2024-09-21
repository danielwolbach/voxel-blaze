#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/rasterizer.hpp>
#include <voxel-blaze/graphics/ray_tracer.hpp>
#include <voxel-blaze/graphics/renderer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>
#include <voxel-blaze/voxels/voxel_grid.hpp>

class Test
{
  public:
    enum class RenderMode
    {
        Rasterize,
        RayTrace
    };

    struct Result
    {
        const Test::RenderMode render_mode;
        const std::string name;
        const glm::ivec3 voxel_grid_size;
        const glm::ivec2 res;
        const unsigned voxel_count;
        const float average_fps;

        inline std::string to_string() const
        {
            const auto mode_str = (render_mode == RenderMode::Rasterize ? "rasterized" : "ray-traced");
            return spdlog::fmt_lib::format(
                "mode: {}, name: {}, res: {}x{}, size: {}x{}x{}, voxel_count: {}, average fps: {}FPS", mode_str, name,
                res.x, res.y, voxel_grid_size.x, voxel_grid_size.y, voxel_grid_size.z, voxel_count, average_fps);
        }
    };

    Test(const std::string &name, std::unique_ptr<VoxelGrid> &&voxel_grid);
    static Test from_vox_path(const std::string &path);
    static Test from_perlin_noise(const float frequency, const unsigned size);
    Result run(float duration, const RenderMode render_mode, const glm::ivec2 res) const;

  private:
    const std::string name;
    const std::unique_ptr<VoxelGrid> voxel_grid;
};