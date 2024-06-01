#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/mesh.hpp>

class Model : Wrapper
{
  public:
    Model(const Mesh &mesh);
    ~Model();
    void rotate(const glm::vec3 angles);
    glm::mat4 get_tranform() const;

  private:
    friend class Renderer;
    unsigned index_buffer;
    unsigned vertex_count;
    unsigned vertex_buffer;
    unsigned vertex_array;
    glm::mat4 transform = glm::mat4(1.0f);
};
