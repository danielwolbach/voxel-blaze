#include <voxel-blaze/graphics/camera.hpp>

OrbitCamera::OrbitCamera(float radius, const glm::vec3 &center) : center(center), radius(radius)
{
}

void OrbitCamera::move(float radius, float theta, float phi)
{
    this->radius += radius;
    this->theta += theta;
    this->phi += phi;
    view_matrix = glm::lookAt(get_position(), center, glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrbitCamera::set(float radius, float theta, float phi)
{
    this->radius = radius;
    this->theta = theta;
    this->phi = phi;
    view_matrix = glm::lookAt(get_position(), center, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 OrbitCamera::get_position() const
{
    float x = radius * glm::sin(theta) * glm::cos(phi);
    float y = radius * glm::cos(theta);
    float z = radius * glm::sin(theta) * glm::sin(phi);
    return glm::vec3(x, y, z) + center;
}

const float *const OrbitCamera::matrix_ptr() const
{
    return glm::value_ptr(view_matrix);
}

glm::vec3 OrbitCamera::get_direction() const
{
    return glm::normalize(center - get_position());
}

glm::vec3 OrbitCamera::get_up() const
{
    glm::vec3 camera_direction = get_direction();
    glm::vec3 camera_right = get_right();
    return glm::normalize(glm::cross(camera_direction, camera_right));
}

glm::vec3 OrbitCamera::get_right() const
{
    glm::vec3 camera_direction = get_direction();
    return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_direction));
}