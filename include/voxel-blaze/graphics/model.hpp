#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/vertex.hpp>

class Model : Wrapper
{
  public:
    Model(const std::vector<unsigned> &indices, const std::vector<Vertex> &vertices);
    ~Model();

  private:
    friend class Renderer;
    unsigned index_buffer;
    unsigned vertex_count;
    unsigned vertex_buffer;
    unsigned vertex_array;
};
