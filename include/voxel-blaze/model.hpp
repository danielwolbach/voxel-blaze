#pragma once

#include <voxel-blaze/common.hpp>

class Shader;

struct Vertex
{
    float x = 0, y = 0, z = 0;
    float r = 1.0, g = 1.0, b = 1.0;

    bool operator==(const Vertex &other) const
    {
        // Use an epsilon for floating-point comparison
        const float epsilon = 1e-5;
        return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon && std::fabs(r - other.r) < epsilon &&
               std::fabs(g - other.g) < epsilon && std::fabs(b - other.b) < epsilon;
    }
};

class Model : Wrapper
{
  public:
    Model(const std::vector<unsigned> &indices, const std::vector<Vertex> &vertices);
    ~Model();

  private:
    friend class Shader;
    unsigned index_buffer;
    unsigned vertex_count;
    unsigned vertex_buffer;
    unsigned vertex_array;
};
