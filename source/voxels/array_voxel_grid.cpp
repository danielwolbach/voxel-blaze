#include <voxel-blaze/voxels/array_voxel_grid.hpp>

ArrayVoxelGrid::ArrayVoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z)
    : VoxelGrid(size_x, size_y, size_z), voxels(size_x * size_y * size_z, std::nullopt)
{
}

std::optional<Voxel> ArrayVoxelGrid::get_voxel(const unsigned x, const unsigned y, const unsigned z) const
{
    if (x > size_x || y > size_y || z > size_z)
    {
        return std::nullopt;
    }

    return voxels[calculate_index(x, y, z)];
}

void ArrayVoxelGrid::set_voxel(const unsigned x, const unsigned y, const unsigned z, const std::optional<Voxel> &voxel)
{
    voxels[calculate_index(x, y, z)] = voxel;
    spdlog::trace("Placed voxel at ({}, {}, {}).", x, y, z);
}

unsigned ArrayVoxelGrid::calculate_index(const unsigned x, const unsigned y, const unsigned z) const
{
    return x + size_x * (y + size_y * z);
}