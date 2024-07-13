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

unsigned VoxelGrid::fill_perlin_noise(const Voxel &voxel, float frequency)
{
    unsigned counter = 0;

    for (unsigned x = 0; x < size_x; x++)
    {
        for (unsigned y = 0; y < size_y; y++)
        {
            for (unsigned z = 0; z < size_z; z++)
            {
                glm::vec3 pos = glm::vec3(x, y, z) * frequency;

                float noise_value = glm::perlin(pos);

                if (noise_value > 0.0f)
                {
                    set_voxel(x, y, z, voxel);
                    counter += 1;
                }
            }
        }
    }

    spdlog::info("Filled a total of {} voxels with Perlin noise.", counter);

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
                    const auto base = Vertex{(float)x - (float)size_x / 2.0f,
                                             (float)y - (float)size_y / 2.0f,
                                             (float)z - (float)size_z / 2.0f,
                                             voxel.r,
                                             voxel.g,
                                             voxel.b};

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

    spdlog::info("Meshified (direct) with {} vertices and {} triangle faces ({} square faces).", vertices.size(),
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
                    const auto base = Vertex{(float)x - (float)size_x / 2.0f,
                                             (float)y - (float)size_y / 2.0f,
                                             (float)z - (float)size_z / 2.0f,
                                             voxel.r,
                                             voxel.g,
                                             voxel.b};

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

struct Face
{
    enum class Direction
    {
        Front,
        Back
    };

    float r, g, b;
    Direction direction;

    inline bool operator==(const Face &other) const
    {
        return std::tie(r, g, b, direction) == std::tie(other.r, other.g, other.b, other.direction);
    }
};

class Mask2D
{
  private:
    std::vector<std::optional<Face>> data;

  public:
    const unsigned size_u;
    const unsigned size_v;

    inline Mask2D(unsigned size_u, unsigned size_v)
        : data(size_u * size_v, std::nullopt), size_u(size_u), size_v(size_v)
    {
    }

    inline void enable(const unsigned u, const unsigned v, const Face face)
    {
        data[u * size_v + v] = face;
    }

    inline void disable(const unsigned u, const unsigned v)
    {
        data[u * size_v + v] = std::nullopt;
    }

    inline std::optional<Face> get(const unsigned u, const unsigned v)
    {
        return data[u * size_v + v];
    }
};

Mesh VoxelGrid::meshify_greedy() const
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::unordered_map<Vertex, unsigned> map;

    for (unsigned dimension = 0; dimension < 3; dimension += 1)
    {
        const unsigned sizes[] = {size_x, size_y, size_z};
        const unsigned u = (dimension + 1) % 3;
        const unsigned v = (dimension + 2) % 3;

        unsigned x[3] = {0, 0, 0};
        unsigned direction[] = {0, 0, 0};
        direction[dimension] = 1;

        for (x[dimension] = 0; x[dimension] <= sizes[dimension]; x[dimension]++)
        {
            Mask2D mask(sizes[u], sizes[v]);
            spdlog::trace("Iterating dimension {} at {}", dimension, x[dimension]);

            for (x[v] = 0; x[v] < sizes[v]; ++x[v])
            {
                for (x[u] = 0; x[u] < sizes[u]; ++x[u])
                {
                    if (x[dimension] == sizes[dimension] || !get_voxel(x[0], x[1], x[2]).has_value())
                    {
                        if (get_voxel(x[0] - direction[0], x[1] - direction[1], x[2] - direction[2]).has_value())
                        {
                            const auto voxel =
                                *get_voxel(x[0] - direction[0], x[1] - direction[1], x[2] - direction[2]);
                            mask.enable(x[u], x[v], {voxel.r, voxel.g, voxel.b, Face::Direction::Back});
                        }
                    }
                    else if (get_voxel(x[0], x[1], x[2]).has_value())
                    {
                        if (x[dimension] == 0 || x[dimension] == sizes[dimension] ||
                            !get_voxel(x[0] - direction[0], x[1] - direction[1], x[2] - direction[2]).has_value())
                        {
                            const auto voxel = *get_voxel(x[0], x[1], x[2]);
                            mask.enable(x[u], x[v], {voxel.r, voxel.g, voxel.b, Face::Direction::Front});
                        }
                    }
                }
            }

            for (unsigned temp_v = 0; temp_v < sizes[v]; ++temp_v)
            {
                for (unsigned temp_u = 0; temp_u < sizes[u]; ++temp_u)
                {
                    auto mask_optional = mask.get(temp_u, temp_v);
                    if (mask_optional.has_value())
                    {
                        unsigned w = 0;
                        for (w = 1; temp_u + w < sizes[u] && mask.get(temp_u + w, temp_v) == mask_optional; w++)
                        {
                        }

                        unsigned h = 0;
                        bool done = false;
                        for (h = 1; temp_v + h < sizes[v]; h++)
                        {
                            for (unsigned k = 0; k < w; k += 1)
                            {
                                if (!(mask.get(temp_u + k, temp_v + h) == mask_optional))
                                {
                                    done = true;
                                    break;
                                }
                            }

                            if (done)
                                break;
                        }

                        x[u] = temp_u;
                        x[v] = temp_v;

                        int du[3] = {0, 0, 0};
                        du[u] = w;

                        int dv[3] = {0, 0, 0};
                        dv[v] = h;

                        float color[3] = {mask_optional->r, mask_optional->g, mask_optional->b};

                        float start_x = -(float)size_x / 2.0f;
                        float start_y = -(float)size_y / 2.0f;
                        float start_z = -(float)size_z / 2.0f;

                        const auto face_vertices = std::vector<Vertex>(
                            {Vertex{start_x + (float)x[0], start_y + (float)x[1], start_z + (float)x[2], color[0],
                                    color[1], color[2]},
                             Vertex{start_x + (float)x[0] + du[0], start_y + (float)x[1] + du[1],
                                    start_z + (float)x[2] + du[2], color[0], color[1], color[2]},
                             Vertex{start_x + (float)x[0] + dv[0], start_y + (float)x[1] + dv[1],
                                    start_z + (float)x[2] + dv[2], color[0], color[1], color[2]},
                             Vertex{start_x + (float)x[0] + du[0] + dv[0], start_y + (float)x[1] + du[1] + dv[1],
                                    start_z + (float)x[2] + du[2] + dv[2], color[0], color[1], color[2]}});

                        const auto front_face_indices = {0, 2, 1, 1, 2, 3};
                        const auto back_face_indices = {0, 1, 2, 1, 3, 2};

                        auto face_indices = front_face_indices;

                        if (mask_optional->direction == Face::Direction::Back)
                        {
                            face_indices = back_face_indices;
                        }

                        for (const auto &index : face_indices)
                        {
                            const auto vertex = face_vertices[index];
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

                        for (unsigned clear_height = 0; clear_height < h; ++clear_height)
                        {
                            for (unsigned k = 0; k < w; ++k)
                            {
                                mask.disable(temp_u + k, temp_v + clear_height);
                            }
                        }

                        spdlog::trace("Found quad at ({}, {}, {}) width size ({}, {})", x[0], x[1], x[2], w, h);
                        temp_u += w - 1;
                    }
                }
            }
        }
    }

    for (const auto &vertex : vertices)
    {
        spdlog::trace("Found vertex {} {} {}", vertex.x, vertex.y, vertex.z);
    }

    spdlog::info("Meshified (greedy) with {} vertices and {} triangle faces ({} square faces).", vertices.size(),
                 indices.size() / 3, indices.size() / 3 / 2);

    return Mesh{indices, vertices};
}
