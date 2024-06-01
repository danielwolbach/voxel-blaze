#pragma once

#include <voxel-blaze/common.hpp>

struct Vertex
{
    float x = 0, y = 0, z = 0;
    float r = 1.0, g = 1.0, b = 1.0;

    inline bool operator==(const Vertex &other) const
    {
        const float epsilon = glm::epsilon<float>();
        return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon && std::fabs(r - other.r) < epsilon &&
               std::fabs(g - other.g) < epsilon && std::fabs(b - other.b) < epsilon;
    }

    static std::vector<Vertex> generate_cube_vertices(const Vertex &base);
    static std::vector<unsigned> generate_cube_indices();
};

namespace std
{
    template <>
    struct hash<Vertex>
    {
        inline std::size_t operator()(const Vertex &vertex) const
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
