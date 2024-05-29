#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/voxels/voxel.hpp>
#include <voxel-blaze/graphics/model.hpp>

class VoxelGrid
{
public:
	VoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z);
	virtual ~VoxelGrid() = default;
	virtual std::optional<Voxel> get_voxel(const unsigned x, const unsigned y, const unsigned z) const = 0;
	virtual void set_voxel(const unsigned x, const unsigned y, const unsigned z, const std::optional<Voxel>& voxel) = 0;
	void fill_cuboid(const Voxel& voxel);
	void fill_ellipsoid(const Voxel& voxel);
	Model meshify_direct() const;

protected:
	const unsigned size_x;
	const unsigned size_y;
	const unsigned size_z;
};