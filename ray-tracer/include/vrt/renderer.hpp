#pragma once
#include <vrt/octree.hpp>

namespace vrt
{
    class Renderer
    {
    public:
        Renderer(const unsigned image_width, const unsigned image_height);
        void render(const Octree &octree) const;
        
    private:
        static void write_pixel(std::ostream &out, const Vec3 &pixel);

        const unsigned image_width;
        const unsigned image_height;
    };
}