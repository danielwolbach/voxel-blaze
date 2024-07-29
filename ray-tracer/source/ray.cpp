#include <vrt/ray.hpp>

namespace vrt
{
    Ray::Ray(const Vec3 &origin, const Vec3 &direction)
        : origin(origin), direction(direction)
    {
    }

    bool Ray::intersects(const Box &box) const
    {
        float t_min = 0.0, t_max = INFINITY;

        for (int d = 0; d < 3; ++d)
        {
            float t1 = (box.get_min()[d] - origin[d]) / direction[d];
            float t2 = (box.get_max()[d] - origin[d]) / direction[d];
            t_min = glm::max(t_min, glm::min(glm::min(t1, t2), t_max));
            t_max = glm::min(t_max, glm::max(glm::max(t1, t2), t_min));
        }

        return t_min < t_max;
    }

    const Vec3 &Ray::get_origin() const
    {
        return origin;
    }
    const Vec3 &Ray::get_direction() const
    {
        return direction;
    }
}