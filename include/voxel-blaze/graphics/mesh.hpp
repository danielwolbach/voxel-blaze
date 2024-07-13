#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/vertex.hpp>

struct Mesh
{
    std::vector<unsigned> indices;
    std::vector<Vertex> vertices;

    inline void save_obj(const std::string &path) const
    {
        std::ofstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file");
        }

        for (const auto &vertex : vertices)
        {
            file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
        }

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            if (i + 2 < indices.size())
            {
                file << "f " << indices[i] + 1 << " " << indices[i + 1] + 1 << " " << indices[i + 2] + 1 << "\n";
            }
        }

        file.close();
    }
};
