#include <voxel-blaze/graphics/vertex.hpp>

std::vector<Vertex> Vertex::generate_cube_vertices(const Vertex &base)
{
    std::vector<Vertex> vertices = {
        {base.x, base.y, base.z, base.r, base.g, base.b}, {base.x + 1.0F, base.y, base.z, base.r, base.g, base.b},
        {base.x + 1.0F, base.y + 1.0F, base.z, base.r, base.g, base.b},
        {base.x, base.y + 1.0F, base.z, base.r, base.g, base.b},
        {base.x, base.y, base.z + 1.0F, base.r, base.g, base.b},
        {base.x + 1.0F, base.y, base.z + 1.0F, base.r, base.g, base.b},
        {base.x + 1.0F, base.y + 1.0F, base.z + 1.0F, base.r, base.g, base.b},
        {base.x, base.y + 1.0F, base.z + 1.0F, base.r, base.g, base.b}};
    return vertices;
}

std::vector<unsigned> Vertex::generate_cube_indices()
{
    std::vector<unsigned> indices = {4, 5, 6, 6, 7, 4, 0, 1, 2, 2, 3, 0, 2, 6, 5, 5, 1, 2,
                                     0, 4, 7, 7, 3, 0, 2, 3, 7, 7, 6, 2, 1, 5, 4, 4, 0, 1};
    return indices;
}