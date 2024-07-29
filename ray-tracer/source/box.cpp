#include <vrt/box.hpp>

namespace vrt
{
    Box::Box(const Vec3 &min, const Vec3 &max)
        : min(min), max(max)
    {
    }

    Vec3 Box::size() const {
        return max - min;
    }

    Vec3 Box::center() const {
        return min + size() / 2.0f;
    }

    const Vec3 &Box::get_min() const
    {
        return min;
    }

    const Vec3 &Box::get_max() const
    {
        return max;
    }
}