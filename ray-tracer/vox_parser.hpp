#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cinttypes>

struct VoxelGrid
{
    uint32_t size_x;
    uint32_t size_y;
    uint32_t size_z;
    std::vector<float> values;
};

class VoxParser
{
  public:
    VoxParser(const std::string &path);
    ~VoxParser() = default;
    VoxelGrid get_voxel_grid() const;

  private:
    struct EntryXYZI
    {
        uint8_t x, y, z, i;
    };

    struct EntryRGBA
    {
        uint8_t r, g, b, a;
    };

    std::streamsize read_chunk(std::ifstream &file);

    uint32_t size_x;
    uint32_t size_y;
    uint32_t size_z;
    std::vector<EntryXYZI> voxels;
    std::vector<EntryRGBA> colors;
};
