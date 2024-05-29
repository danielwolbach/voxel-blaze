#include <voxel-blaze/graphics/camera.hpp>

void Camera::look_at(const glm::vec3 &position, const glm::vec3 &target)
{
    view_matrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

 const float *const Camera::matrix_ptr() const
 {
    return glm::value_ptr(view_matrix);
 }