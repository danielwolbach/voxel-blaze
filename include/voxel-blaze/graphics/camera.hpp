#pragma once

#include <voxel-blaze/common.hpp>

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;
    void look_at(const glm::vec3 &position, const glm::vec3 &target);
    const float *const matrix_ptr() const;

private:
    glm::mat4 view_matrix;
};