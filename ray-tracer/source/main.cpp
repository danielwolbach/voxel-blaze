#include <vrt/common.hpp>
#include <vrt/ray.hpp>
#include <vrt/box.hpp>
#include <vrt/renderer.hpp>
#include <vrt/octree.hpp>

int main()
{
    vrt::Octree octree(5);
    octree.place_random(10);
    octree.insert(vrt::Vec3(15, 15, 15), vrt::Voxel{});
    octree.insert(vrt::Vec3(0, 0, 0), vrt::Voxel{});

    auto aspect_ratio = 16.0 / 9.0;
    auto image_width = 1600;

    vrt::Renderer renderer(image_width, image_width / aspect_ratio);

    renderer.render(octree);
}