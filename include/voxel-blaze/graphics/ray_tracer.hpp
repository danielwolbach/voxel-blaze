#pragma once

#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/camera.hpp>
#include <voxel-blaze/common.hpp>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/renderer.hpp>

class RayTracer : public Renderer
{
public:
    RayTracer(const VoxelGrid &voxel_grid);
    ~RayTracer();
    virtual void render(const OrbitCamera &camera) const;

private:
    unsigned tracer_program;
    unsigned screen_program;
    unsigned vertex_array;
    unsigned vertex_buffer;
    unsigned element_buffer;
    unsigned voxel_grid_texture;
    unsigned screen_texture;
};