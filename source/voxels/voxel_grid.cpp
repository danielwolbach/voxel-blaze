#include <voxel-blaze/voxels/voxel_grid.hpp>

VoxelGrid::VoxelGrid(const unsigned size_x, const unsigned size_y, const unsigned size_z)
    : size_x(size_x), size_y(size_y), size_z(size_z)
{
}

unsigned VoxelGrid::max_size() const
{
    return std::max({size_x, size_y, size_z});
}

unsigned VoxelGrid::fill_cuboid(const Voxel &voxel)
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

    return counter;
}

unsigned VoxelGrid::fill_ellipsoid(const Voxel &voxel)
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

    return counter;
}

Mesh VoxelGrid::meshify_direct() const
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
                const auto voxel_optional = get_voxel(x, y, z);

                if (voxel_optional.has_value())
                {
                    const auto voxel = voxel_optional.value();
                    const auto base = Vertex 
                    {
                        (float)x - (float)size_x / 2.0f,
                        (float)y - (float)size_y / 2.0f,
                        (float)z - (float)size_z / 2.0f,
                        voxel.r,
                        voxel.g,
                        voxel.b
                    };
                    
                    const auto single_vertices = Vertex::generate_cube_vertices(base);
                    const auto single_indices = Vertex::generate_cube_indices();

                    for (const auto i : single_indices)
                    {
                        const auto vertex = single_vertices[i];
                        const auto it = map.find(vertex);

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

    spdlog::info("Meshified (culled) with {} vertices and {} triangle faces ({} square faces).", vertices.size(),
                 indices.size() / 3, indices.size() / 3 / 2);

    return Mesh{indices, vertices};
}

Mesh VoxelGrid::meshify_culled() const
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
                const auto voxel_optional = get_voxel(x, y, z);

                if (voxel_optional.has_value())
                {
                    const auto voxel = voxel_optional.value();
                    const auto base = Vertex 
                    {
                        (float)x - (float)size_x / 2.0f,
                        (float)y - (float)size_y / 2.0f,
                        (float)z - (float)size_z / 2.0f,
                        voxel.r,
                        voxel.g,
                        voxel.b
                    };

                    const auto single_vertices = Vertex::generate_cube_vertices(base);

                    const auto positive_x_face = {2, 6, 5, 5, 1, 2};
                    const auto negative_x_face = {0, 4, 7, 7, 3, 0};
                    const auto positive_y_face = {2, 3, 7, 7, 6, 2};
                    const auto negative_y_face = {1, 5, 4, 4, 0, 1};
                    const auto positive_z_face = {4, 5, 6, 6, 7, 4};
                    const auto negative_z_face = {0, 2, 1, 2, 0, 3};

                    auto single_indices = std::vector<unsigned>{};

                    if (x == size_x - 1 || !get_voxel(x + 1, y, z).has_value())
                    {
                        single_indices.insert(single_indices.end(), positive_x_face.begin(), positive_x_face.end());
                    }

                    if (x == 0 || !get_voxel(x - 1, y, z).has_value())
                    {
                        single_indices.insert(single_indices.end(), negative_x_face.begin(), negative_x_face.end());
                    }

                    if (y == size_y - 1 || !get_voxel(x, y + 1, z).has_value())
                    {
                        single_indices.insert(single_indices.end(), positive_y_face.begin(), positive_y_face.end());
                    }

                    if (y == 0 || !get_voxel(x, y - 1, z).has_value())
                    {
                        single_indices.insert(single_indices.end(), negative_y_face.begin(), negative_y_face.end());
                    }

                    if (z == size_z - 1 || !get_voxel(x, y, z + 1).has_value())
                    {
                        single_indices.insert(single_indices.end(), positive_z_face.begin(), positive_z_face.end());
                    }

                    if (z == 0 || !get_voxel(x, y, z - 1).has_value())
                    {
                        single_indices.insert(single_indices.end(), negative_z_face.begin(), negative_z_face.end());
                    }

                    for (const auto i : single_indices)
                    {
                        const auto vertex = single_vertices[i];
                        const auto it = map.find(vertex);

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

    spdlog::info("Meshified (culled) with {} vertices and {} triangle faces ({} square faces).", vertices.size(),
                 indices.size() / 3, indices.size() / 3 / 2);

    return Mesh{indices, vertices};
}