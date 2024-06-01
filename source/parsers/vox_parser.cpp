#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>

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
        for (uint32_t default_color : consts::vox_default_palette)
        {
            const uint8_t r = (default_color >> 24) & 0xFF;
            const uint8_t g = (default_color >> 16) & 0xFF;
            const uint8_t b = (default_color >> 8) & 0xFF;
            const uint8_t a = default_color & 0xFF;
            colors.push_back({r, g, b, a});
        }
    }
}

std::unique_ptr<VoxelGrid> VoxParser::get_voxel_grid() const
{
    // Create voxel grid.
    std::unique_ptr<VoxelGrid> voxel_grid = std::make_unique<ArrayVoxelGrid>(size_x, size_y, size_z);
    
    for (const auto voxel_entry : voxels)
    {
        spdlog::trace("Found voxel entry {} {} {} {}", voxel_entry.x, voxel_entry.y, voxel_entry.z, voxel_entry.i);
        const auto color_entry = colors[voxel_entry.i];
        voxel_grid->set_voxel(voxel_entry.x, voxel_entry.y, voxel_entry.z,
                             Voxel{color_entry.r / 255.0f, color_entry.g / 255.0f, color_entry.b / 255.0f});
    }

    return voxel_grid;
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
        file.read(reinterpret_cast<char *>(&size_y), sizeof(size_y));
        file.read(reinterpret_cast<char *>(&size_z), sizeof(size_z));

        spdlog::trace("Found size {}x{}x{}", size_x, size_y, size_z);
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
            file.read(reinterpret_cast<char *>(&voxel_y), sizeof(voxel_y));
            file.read(reinterpret_cast<char *>(&voxel_z), sizeof(voxel_z));
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