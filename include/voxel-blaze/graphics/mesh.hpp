#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/vertex.hpp>

struct Mesh
{
    std::vector<unsigned> indices;
    std::vector<Vertex> vertices;
};