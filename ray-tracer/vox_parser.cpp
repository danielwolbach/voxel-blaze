#include "vox_parser.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

const unsigned int vox_default_palette[256] = {
        0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff,
        0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff, 0xff6699ff, 0xff3399ff,
        0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff,
        0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff, 0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff,
        0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc,
        0xff99cccc, 0xff66cccc, 0xff33cccc, 0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc,
        0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc,
        0xff9933cc, 0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc,
        0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99, 0xffcccc99,
        0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999,
        0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699, 0xff006699, 0xffff3399, 0xffcc3399,
        0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099,
        0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66, 0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66,
        0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966,
        0xff009966, 0xffff6666, 0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366,
        0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
        0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33,
        0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933, 0xff669933, 0xff339933,
        0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333,
        0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033, 0xffcc0033, 0xff990033, 0xff660033, 0xff330033,
        0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00,
        0xff99cc00, 0xff66cc00, 0xff33cc00, 0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900,
        0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300,
        0xff993300, 0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000,
        0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044, 0xff000022,
        0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400,
        0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000, 0xff880000, 0xff770000, 0xff550000,
        0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777,
        0xff555555, 0xff444444, 0xff222222, 0xff111111};

VoxParser::VoxParser(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);

    // Check header.
    char header[4];
    file.read(header, sizeof(header) / sizeof(*header));
    if (header[0] != 'V' || header[1] != 'O' || header[2] != 'X' || header[3] != ' ')
    {
        std::cerr << "Error: Invalid .vox file format." << std::endl;
    }

    // Check version.
    uint32_t version;
    file.read(reinterpret_cast<char *>(&version), sizeof(version));

    const auto read_bytes = read_chunk(file) + 8;
    spdlog::info("Read VOX file with a size of {}B", read_bytes);
    file.close();

    // Use default pallette if necessary
    if (colors.empty())
    {
        colors.reserve(256);
        for (uint32_t default_color : vox_default_palette)
        {
            const uint8_t r = (default_color >> 24) & 0xFF;
            const uint8_t g = (default_color >> 16) & 0xFF;
            const uint8_t b = (default_color >> 8) & 0xFF;
            const uint8_t a = default_color & 0xFF;
            colors.push_back({r, g, b, a});
        }
    }
}

VoxelGrid VoxParser::get_voxel_grid() const
{
    std::vector<float> voxel_grid(size_x * size_y * size_z * 4, 0.0f);

    // Create voxel grid.
    for (const auto voxel_entry : voxels)
    {
        spdlog::trace("Found voxel entry {} {} {} {}", voxel_entry.x, voxel_entry.y, voxel_entry.z, voxel_entry.i);
        const auto color_entry = colors[voxel_entry.i];

        auto index = (voxel_entry.x + size_x * (voxel_entry.y + size_y * voxel_entry.z)) * 4;
        voxel_grid[index + 0] = (color_entry.r / 255.0f);
        voxel_grid[index + 1] = (color_entry.g / 255.0f);
        voxel_grid[index + 2] = (color_entry.b / 255.0f);
        voxel_grid[index + 3] = (color_entry.a / 255.0f);
        // voxel_grid->set_voxel(voxel_entry.x, voxel_entry.y, voxel_entry.z,
                            //  Voxel{color_entry.r / 255.0f, color_entry.g / 255.0f, color_entry.b / 255.0f});
    }

    return {size_x, size_y, size_z, voxel_grid};
}

std::streamsize VoxParser::read_chunk(std::ifstream &file)
{
    // Keep track of read bytes.
    std::streampos start_byte_pos = file.tellg();

    // Read chunk metadata.
    char chunk_id[4];
    uint32_t chunk_data_size;
    uint32_t chunk_children_size;
    file.read(chunk_id, sizeof(chunk_id) / sizeof(*chunk_id));
    file.read(reinterpret_cast<char *>(&chunk_data_size), sizeof(chunk_data_size));
    file.read(reinterpret_cast<char *>(&chunk_children_size), sizeof(chunk_children_size));

    spdlog::trace("Found chunk {}{}{}{} with data size of {}B and children size of {}B", chunk_id[0], chunk_id[1],
                  chunk_id[2], chunk_id[3], chunk_data_size, chunk_children_size);

    if (chunk_id[0] == 'S' && chunk_id[1] == 'I' && chunk_id[2] == 'Z' && chunk_id[3] == 'E')
    {
        // Extract chunk size information.
        file.read(reinterpret_cast<char *>(&size_x), sizeof(size_x));
        file.read(reinterpret_cast<char *>(&size_z), sizeof(size_z));
        file.read(reinterpret_cast<char *>(&size_y), sizeof(size_y));

        spdlog::info("Found size {}x{}x{}", size_x, size_y, size_z);
    }
    else if (chunk_id[0] == 'X' && chunk_id[1] == 'Y' && chunk_id[2] == 'Z' && chunk_id[3] == 'I')
    {
        // Extract voxel information.
        uint32_t voxel_count;
        file.read(reinterpret_cast<char *>(&voxel_count), sizeof(voxel_count));
        voxels.reserve(voxel_count);

        for (unsigned i = 0; i < voxel_count; i++)
        {
            uint8_t voxel_x = 0;
            uint8_t voxel_y = 0;
            uint8_t voxel_z = 0;
            uint8_t voxel_i = 0;
            file.read(reinterpret_cast<char *>(&voxel_x), sizeof(voxel_x));
            file.read(reinterpret_cast<char *>(&voxel_z), sizeof(voxel_z));
            file.read(reinterpret_cast<char *>(&voxel_y), sizeof(voxel_y));
            file.read(reinterpret_cast<char *>(&voxel_i), sizeof(voxel_i));

            spdlog::trace("Found voxel with coordinates ({}, {}, {}, {})", voxel_x, voxel_y, voxel_z, voxel_i);
            voxels.push_back({voxel_x, voxel_y, voxel_z, voxel_i});
        }
    }
    else if (chunk_id[0] == 'R' && chunk_id[1] == 'G' && chunk_id[2] == 'B' && chunk_id[3] == 'A')
    {
        // Extract color information.
        colors.reserve(256);
        for (unsigned i = 0; i < 256; i++)
        {
            uint8_t color_r = 0;
            uint8_t color_g = 0;
            uint8_t color_b = 0;
            uint8_t color_a = 0;
            file.read(reinterpret_cast<char *>(&color_r), sizeof(color_r));
            file.read(reinterpret_cast<char *>(&color_g), sizeof(color_g));
            file.read(reinterpret_cast<char *>(&color_b), sizeof(color_b));
            file.read(reinterpret_cast<char *>(&color_a), sizeof(color_a));

            spdlog::trace("Found color #{} with RGBA values ({}, {}, {}, {})", i, color_r, color_g, color_b, color_a);
            colors.push_back({color_r, color_g, color_b, color_a});
        }
    }
    else
    {
        // Skip data block of unneeded tags.
        file.seekg(chunk_data_size, std::ios::cur);
    }

    while (chunk_children_size > 0)
    {
        // Recurse children tags.
        const auto read_rec = read_chunk(file);
        chunk_children_size -= read_rec;
    }

    // Calculate read bytes.
    std::streampos end_pos = file.tellg();
    std::streamsize bytes_read = end_pos - start_byte_pos;

    return bytes_read;
}