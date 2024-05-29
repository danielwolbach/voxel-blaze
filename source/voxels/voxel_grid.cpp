#include <voxel-blaze/voxels/voxel_grid.hpp>

VoxelGrid::VoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z)
    : size_x(size_x), size_y(size_y), size_z(size_z)
{
}

void VoxelGrid::fill_cuboid(const Voxel &voxel)
{
    unsigned counter = 0;

    for (unsigned x = 0; x < size_x; x++)
    {
        for (unsigned y = 0; y < size_y; y++)
        {
            for (unsigned z = 0; z < size_z; z++)
            {
                set_voxel(x, y, z, voxel);
                counter += 1;
            }
        }
    }

    spdlog::info("Filled a total of {} voxels.", counter);
}

void VoxelGrid::fill_ellipsoid(const Voxel &voxel)
{
    unsigned counter = 0;

    const float center_x = size_x / 2.0f;
    const float center_y = size_y / 2.0f;
    const float center_z = size_z / 2.0f;
    const float radius_x = size_x / 2.0f;
    const float radius_y = size_y / 2.0f;
    const float radius_z = size_z / 2.0f;

    for (unsigned x = 0; x < size_x; x++)
    {
        for (unsigned y = 0; y < size_y; y++)
        {
            for (unsigned z = 0; z < size_z; z++)
            {
                float dx = (x - center_x) / radius_x;
                float dy = (y - center_y) / radius_y;
                float dz = (z - center_z) / radius_z;

                if (dx * dx + dy * dy + dz * dz <= 1.0f)
                {
                    set_voxel(x, y, z, voxel);
                    counter += 1;
                }
            }
        }
    }

    spdlog::info("Filled a total of {} voxels.", counter);
}

Model VoxelGrid::meshify_direct() const
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::unordered_map<Vertex, unsigned> map;

    for (unsigned x = 0; x < size_x; x++)
    {
        for (unsigned y = 0; y < size_y; y++)
        {
            for (unsigned z = 0; z < size_z; z++)
            {
                auto voxel_optional = get_voxel(x, y, z);

                if (voxel_optional.has_value())
                {
                    auto single_vertices = Vertex::generate_cube_vertices(x, y, z);
                    auto single_indices = Vertex::generate_cube_indices();

                    for (auto i : single_indices)
                    {
                        auto vertex = single_vertices[i];
                        auto it = map.find(vertex);

                        if (it != map.end())
                        {
                            indices.push_back(it->second);
                        }
                        else
                        {
                            map[vertex] = vertices.size();
                            indices.push_back(vertices.size());
                            vertices.push_back(vertex);
                        }
                    }
                }
            }
        }
    }

    spdlog::info("Meshified (direct) with {} vertices and {} faces.", vertices.size(), indices.size() / 3);

	return Model(indices, vertices);
}