#pragma once

#include <voxel-blaze/common.hpp>

class OrbitCamera
{
public:
    OrbitCamera(float radius);
    ~OrbitCamera() = default;
    const float *const matrix_ptr() const;
    void move(float theta, float phi);
    glm::vec3 get_position() const;
    glm::vec3 get_direction() const;
    glm::vec3 get_up() const;

private:
    float radius = 0.0f;
    float theta = glm::radians(90.0f);
    float phi = glm::radians(0.0f);
    glm::mat4 view_matrix = glm::lookAt(get_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));;
};