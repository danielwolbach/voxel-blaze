#pragma once

#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/camera.hpp>
#include <voxel-blaze/common.hpp>

class Renderer
{
public:
    Renderer(Shader &&shader);
    ~Renderer() = default;

    float draw(const Camera &camera, const Model &model);

private:
    const Shader shader;
    std::chrono::system_clock::time_point last_time;
};