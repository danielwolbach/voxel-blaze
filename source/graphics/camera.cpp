#include <voxel-blaze/graphics/camera.hpp>


OrbitCamera::OrbitCamera(float radius) : radius(radius)
{
}

void OrbitCamera::move(float theta, float phi)
{
    this->theta += theta;
    this->phi += phi;
    view_matrix = glm::lookAt(get_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 OrbitCamera::get_position() const
{
    float x = radius * glm::sin(theta) * glm::cos(phi);
    float y = radius * glm::cos(theta);
    float z = radius * glm::sin(theta) * glm::sin(phi);

    return glm::vec3(x, y, z);
}

const float *const OrbitCamera::matrix_ptr() const
{
    return glm::value_ptr(view_matrix);
}

glm::vec3 OrbitCamera::get_direction() const
{
    return glm::normalize(glm::vec3(0, 0, 0) - get_position());
}

glm::vec3 OrbitCamera::get_up() const
{
    return glm::vec3(0, 1.0, 0);
}