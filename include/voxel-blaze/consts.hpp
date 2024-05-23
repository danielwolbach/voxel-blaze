#pragma once

#include <voxel-blaze/common.hpp>

const std::vector<float> cube_vertices = {
    // Front face
    -0.5f, -0.5f, 0.5f, // Position
    1.0f, 0.0f, 0.0f,   // Color
    0.5f, -0.5f, 0.5f,  // Position
    1.0f, 0.0f, 0.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    1.0f, 0.0f, 0.0f,   // Color
    -0.5f, 0.5f, 0.5f,  // Position
    1.0f, 0.0f, 0.0f,   // Color

    // Back face
    -0.5f, -0.5f, -0.5f, // Position
    0.0f, 1.0f, 0.0f,    // Color
    0.5f, -0.5f, -0.5f,  // Position
    0.0f, 1.0f, 0.0f,    // Color
    0.5f, 0.5f, -0.5f,   // Position
    0.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, -0.5f,  // Position
    0.0f, 1.0f, 0.0f,    // Color

    // Right face
    0.5f, -0.5f, 0.5f,  // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, -0.5f, -0.5f, // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, 0.5f, -0.5f,  // Position
    0.0f, 0.0f, 1.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    0.0f, 0.0f, 1.0f,   // Color

    // Left face
    -0.5f, -0.5f, 0.5f,  // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, -0.5f, -0.5f, // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, -0.5f,  // Position
    1.0f, 1.0f, 0.0f,    // Color
    -0.5f, 0.5f, 0.5f,   // Position
    1.0f, 1.0f, 0.0f,    // Color

    // Top face
    -0.5f, 0.5f, 0.5f,  // Position
    0.0f, 1.0f, 1.0f,   // Color
    0.5f, 0.5f, 0.5f,   // Position
    0.0f, 1.0f, 1.0f,   // Color
    0.5f, 0.5f, -0.5f,  // Position
    0.0f, 1.0f, 1.0f,   // Color
    -0.5f, 0.5f, -0.5f, // Position
    0.0f, 1.0f, 1.0f,   // Color

    // Bottom face
    -0.5f, -0.5f, 0.5f,  // Position
    1.0f, 0.0f, 1.0f,    // Color
    0.5f, -0.5f, 0.5f,   // Position
    1.0f, 0.0f, 1.0f,    // Color
    0.5f, -0.5f, -0.5f,  // Position
    1.0f, 0.0f, 1.0f,    // Color
    -0.5f, -0.5f, -0.5f, // Position
    1.0f, 0.0f, 1.0f     // Color
};

const std::vector<unsigned> cube_indices = {
    0,  1,  2,  2,  3,  0,  // Front face
    4,  5,  6,  6,  7,  4,  // Back face
    8,  9,  10, 10, 11, 8,  // Right face
    12, 13, 14, 14, 15, 12, // Left face
    16, 17, 18, 18, 19, 16, // Top face
    20, 21, 22, 22, 23, 20  // Bottom face
};
