#pragma once
#include <vrt/common.hpp>

namespace vrt
{
    class Box
    {
    public:
        Box() = default;
        Box(const Box &other) = default;
        Box(const Vec3 &min, const Vec3 &max);

        Vec3 size() const;
        Vec3 center() const;

        const Vec3 &get_min() const;
        const Vec3 &get_max() const;

    private:
        Vec3 min = Vec3(0.0f);
        Vec3 max = Vec3(0.0f);
    };
}