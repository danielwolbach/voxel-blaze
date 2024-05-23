#include <voxel-blaze/voxel.hpp>

namespace std
{
    template <>
    struct hash<Vertex>
    {
        std::size_t operator()(const Vertex &vertex) const
        {
            std::size_t h1 = std::hash<float>()(vertex.x);
            std::size_t h2 = std::hash<float>()(vertex.y);
            std::size_t h3 = std::hash<float>()(vertex.z);
            std::size_t h4 = std::hash<float>()(vertex.r);
            std::size_t h5 = std::hash<float>()(vertex.g);
            std::size_t h6 = std::hash<float>()(vertex.b);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
        }
    };
}

Chunk::Chunk(unsigned size) : size_x(size), size_y(size), size_z(size)
{
    for (unsigned i = 0; i < size * size * size; i++)
    {
        data.push_back(std::nullopt);
    }
}

unsigned Chunk::index(unsigned x, unsigned y, unsigned z) const
{
    return x + size_x * (y + size_y * z);
}

Voxel Chunk::get(unsigned x, unsigned y, unsigned z) const
{
    return data[index(x, y, z)];
}

void Chunk::set(unsigned x, unsigned y, unsigned z, Voxel voxel)
{
    data[index(x, y, z)] = voxel;
}

Model Chunk::meshify_naive() const
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
                Voxel voxel = get(x, y, z);
                if (voxel.has_value())
                {
                    // Color color = *voxel;
                    float fx = (float)x;
                    float fy = (float)y;
                    float fz = (float)z;
                    std::vector<Vertex> cube_vertices = {{fx, fy, fz, 0.5, 0.5, 0.5},
                                                         {fx + 1.0F, fy, fz, 1.0, 0.5, 0.5},
                                                         {fx + 1.0F, fy + 1.0F, fz, 1.0, 1.0, 0.5},
                                                         {fx, fy + 1.0F, fz, 0.5, 1.0, 0.5},
                                                         {fx, fy, fz + 1.0F, 0.5, 0.5, 1.0},
                                                         {fx + 1.0F, fy, fz + 1.0F, 1.0, 0.5, 1.0},
                                                         {fx + 1.0F, fy + 1.0F, fz + 1.0F, 1.0, 1.0, 1.0},
                                                         {fx, fy + 1.0F, fz + 1.0F, 0.5, 1.0, 1.0}};

                    std::vector<unsigned> cube_indice = {4, 5, 6, 6, 7, 4, 0, 1, 2, 2, 3, 0, 2, 6, 5, 5, 1, 2,
                                                         0, 4, 7, 7, 3, 0, 2, 3, 7, 7, 6, 2, 1, 5, 4, 4, 0, 1};

                    for (unsigned i = 0; i < cube_indice.size(); i++)
                    {
                        unsigned cube_index = cube_indice[i];
                        Vertex vertex = cube_vertices[cube_index];
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

    return Model(indices, vertices);
}

glm::vec3 Chunk::center() const
{
    return glm::vec3(size_x / 2.0F, size_y / 2.0F, size_z / 2.0F);
}

Chunk Chunk::spherical(unsigned diameter)
{
    Chunk chunk(diameter);

    float radius = diameter / 2.0f;
    glm::vec3 center(diameter / 2.0f, diameter / 2.0f, diameter / 2.0f);
    for (unsigned x = 0; x < diameter; x++)
    {
        for (unsigned y = 0; y < diameter; y++)
        {
            for (unsigned z = 0; z < diameter; z++)
            {
                glm::vec3 current(x, y, z);
                float distance = glm::distance(current, center);
                if (distance <= radius)
                {
                    chunk.set(x, y, z, glm::vec3(0.0F, 0.0F, 0.0F));
                }
            }
        }
    }

    return chunk;
}