#include <vrt/renderer.hpp>
#include <vrt/common.hpp>

namespace vrt
{
    Renderer::Renderer(const unsigned image_width, const unsigned image_height)
        : image_width(image_width), image_height(image_height)
    {
    }

    void Renderer::render(const Octree &octree) const
    {
        // Configure the camera.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
        auto camera_center = glm::vec3(0, 0, 0);

        // Calculate the vectors of the viewport edges.
        auto viewport_u = glm::vec3(viewport_width, 0, 0);
        auto viewport_v = glm::vec3(0, -viewport_height, 0);

        // Calculate the pixel-to-pixel vectors.
        auto pixel_delta_u = viewport_u * (1.0f / image_width);
        auto pixel_delta_v = viewport_v * (1.0f / image_height);

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u * 0.5f - viewport_v * 0.5f;
        auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

        // Create a random device and seed the random number generator
        std::random_device random_device;
        std::mt19937 gen(random_device());
        std::uniform_real_distribution<> dis(-0.5, +0.5);

        // Begin PPM P3 file output.
        std::cout << "P3\n";
        std::cout << image_width << " " << image_height << "\n";
        std::cout << "255\n";

        // Render each pixel
        std::vector<Vec3> colors(image_width * image_height);
        for (unsigned j = 0; j < image_height; j++)
        {
            std::clog << "\r" << (unsigned(float(j) / image_height * 100)) << "%" << std::flush;

            for (unsigned i = 0; i < image_width; i++)
            {
                auto color = Vec3(0, 0, 0);
                const auto sample_count = 100U;

                auto offset_x = 0.0f;
                auto offset_y = 0.0f;

                for (unsigned s = 0; s < sample_count; s++)
                {
                    auto pixel_center = pixel00_loc + ((float(i) + offset_x) * pixel_delta_u) + ((float(j) + offset_y) * pixel_delta_v);
                    auto ray_direction = pixel_center - camera_center;
                    auto ray = Ray(camera_center, ray_direction);
                    auto center_value = octree.raycast(ray);

                    if (center_value.has_value())
                    {
                        color += center_value.value().color;
                    }

                    offset_x = float(dis(gen));
                    offset_y = float(dis(gen));
                }

                colors[j * image_width + i] = (1.0f / sample_count) * color;
            }
        }

        for (const auto &color : colors)
        {
            write_pixel(std::cout, color);
        }

        std::clog << "\r100%\n";
    }

    void
    Renderer::write_pixel(std::ostream &out, const Vec3 &pixel_color)
    {
        auto r = pixel_color.x;
        auto g = pixel_color.y;
        auto b = pixel_color.z;
        int r_byte = int(255.999 * r);
        int g_byte = int(255.999 * g);
        int b_byte = int(255.999 * b);
        out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
    }
}