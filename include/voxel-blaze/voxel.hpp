#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/model.hpp>

using Voxel = std::optional<glm::vec3>;

class Chunk
{
  public:
    Chunk(unsigned size);

    unsigned index(unsigned x, unsigned y, unsigned z) const;

    Voxel get(unsigned x, unsigned y, unsigned z) const;

    void set(unsigned x, unsigned y, unsigned z, Voxel voxel);

    Model meshify_naive() const;

    glm::vec3 center() const;

    static Chunk spherical(unsigned diameter);

  private:
    unsigned size_x;
    unsigned size_y;
    unsigned size_z;
    std::vector<Voxel> data;
};
