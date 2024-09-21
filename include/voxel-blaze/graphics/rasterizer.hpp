#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/camera.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/renderer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>

class Rasterizer : public Renderer
{
  public:
    Rasterizer(const VoxelGrid &voxel_grid, const Window &window);
    ~Rasterizer() = default;

    virtual void render(const OrbitCamera &camera) const;

  private:
    const Model model;
    const Shader shader;
};