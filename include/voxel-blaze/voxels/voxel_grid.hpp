#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/mesh.hpp>
#include <voxel-blaze/voxels/voxel.hpp>

class VoxelGrid
{
  public:
    VoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z);
    virtual ~VoxelGrid() = default;
    virtual std::optional<Voxel> get_voxel(const unsigned x, const unsigned y, const unsigned z) const = 0;
    virtual void set_voxel(const unsigned x, const unsigned y, const unsigned z, const std::optional<Voxel> &voxel) = 0;
    unsigned fill_cuboid(const Voxel &voxel);
    unsigned fill_ellipsoid(const Voxel &voxel);
    unsigned max_size() const;
    Mesh meshify_direct() const;
    Mesh meshify_culled() const;

  protected:
    const unsigned size_x;
    const unsigned size_y;
    const unsigned size_z;
};