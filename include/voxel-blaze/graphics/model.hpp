#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/mesh.hpp>

class Model : Wrapper
{
  public:
    Model(const Mesh &mesh);
    ~Model();

  private:
    friend class Rasterizer;
    unsigned index_buffer;
    unsigned vertex_count;
    unsigned vertex_buffer;
    unsigned vertex_array;
};
