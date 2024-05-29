#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/voxels/voxel_grid.hpp>

class ArrayVoxelGrid : public VoxelGrid
{
public:
	ArrayVoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z);
	virtual ~ArrayVoxelGrid() = default;
	virtual std::optional<Voxel> get_voxel(const unsigned x, const unsigned y, const unsigned z) const;
	virtual void set_voxel(const unsigned x, const unsigned y, const unsigned z, const std::optional<Voxel>& voxel);

private:
	unsigned calculate_index(const unsigned x, const unsigned y, const unsigned z) const;
	std::vector<std::optional<Voxel>> voxels;
};