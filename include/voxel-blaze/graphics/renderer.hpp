#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/camera.hpp>
#include <voxel-blaze/voxels/voxel_grid.hpp>

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;
    virtual void render(const OrbitCamera &camera) const = 0;
};