#pragma once
#include <vrt/common.hpp>
#include <vrt/box.hpp>

namespace vrt
{
    class Ray
    {
    public:
        Ray(const Vec3 &origin, const Vec3 &direction);

        bool intersects(const Box &box) const;

        const Vec3 &get_origin() const;
        const Vec3 &get_direction() const;

    private:
        const Vec3 origin;
        const Vec3 direction;
    };
}