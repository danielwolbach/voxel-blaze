#include <voxel-blaze/graphics/vertex.hpp>

std::vector<Vertex> Vertex::generate_cube_vertices(const float offset_x, const float offset_y, const float offset_z)
{
    std::vector<Vertex> vertices = {{offset_x, offset_y, offset_z, 1.0, 1.0, 1.0},
                                    {offset_x + 1.0F, offset_y, offset_z, 1.0, 1.0, 1.0},
                                    {offset_x + 1.0F, offset_y + 1.0F, offset_z, 1.0, 1.0, 1.0},
                                    {offset_x, offset_y + 1.0F, offset_z, 1.0, 1.0, 1.0},
                                    {offset_x, offset_y, offset_z + 1.0F, 1.0, 1.0, 1.0},
                                    {offset_x + 1.0F, offset_y, offset_z + 1.0F, 1.0, 1.0, 1.0},
                                    {offset_x + 1.0F, offset_y + 1.0F, offset_z + 1.0F, 1.0, 1.0, 1.0},
                                    {offset_x, offset_y + 1.0F, offset_z + 1.0F, 1.0, 1.0, 1.0}};
    return vertices;
}

std::vector<unsigned> Vertex::generate_cube_indices()
{
    std::vector<unsigned> indices = {4, 5, 6, 6, 7, 4, 0, 1, 2, 2, 3, 0, 2, 6, 5, 5, 1, 2,
                                     0, 4, 7, 7, 3, 0, 2, 3, 7, 7, 6, 2, 1, 5, 4, 4, 0, 1};
    return indices;
}