#include <vrt/octree.hpp>

namespace vrt
{
    Octree::Octree(unsigned depth)
        : depth(depth)
    {
    }

    void Octree::insert(const Vec3 &location, const Voxel &voxel)
    {
        root.insert(voxel, location, 0, depth);
    }

    void Octree::place_random(const unsigned chance)
    {
        std::random_device random_device;
        std::mt19937 gen(random_device());
        std::uniform_int_distribution<> placing_random(1, chance);
        std::uniform_real_distribution<> color_random(0.0, 1.0);

        for (unsigned x = 0; x < 1U << depth; x++)
        {
            for (unsigned y = 0; y < 1U << depth; y++)
            {
                for (unsigned z = 0; z < 1U << depth; z++)
                {
                    if (placing_random(gen) == 1)
                    {
                        insert(vrt::Vec3(x, y, z), vrt::Voxel{vrt::Vec3(color_random(gen), color_random(gen), color_random(gen))});
                    }
                }
            }
        }
    }

    std::optional<Voxel> Octree::get(const Vec3 &location) const
    {
        return root.get(location, 0, depth);
    }

    std::optional<Voxel> Octree::raycast(const Ray &ray) const
    {
        Vec3 origin(0.0f, 0.0f, -1.1f * float(1 << depth));
        Vec3 min = Vec3(glm::pow(2.0f, depth) / -2.0f) + origin;
        Vec3 max = Vec3(glm::pow(2.0f, depth) / 2.0f) + origin;

        return root.cast_ray(ray, Box(min, max), 0, depth);
    }

    void Octree::Node::insert(const Voxel &voxel, Vec3 location, unsigned current_depth, unsigned max_depth)
    {
        if (current_depth == max_depth + 1)
        {
            this->voxel = voxel;
            return;
        }

        unsigned half_size = (1 << (max_depth - current_depth)) / 2;
        unsigned index = 0;

        if (location.x >= half_size)
        {
            index |= (1 << 2);
            location.x -= half_size;
        }

        if (location.y >= half_size)
        {
            index |= (1 << 1);
            location.y -= half_size;
        }

        if (location.z >= half_size)
        {
            index |= (1 << 0);
            location.z -= half_size;
        }

        if (!children[index])
        {
            children[index] = std::make_unique<Node>();
        }

        children[index]->insert(voxel, location, current_depth + 1, max_depth);
    }

    std::optional<Voxel> Octree::Node::get(Vec3 location, unsigned current_depth, unsigned max_depth) const
    {
        if (current_depth == max_depth + 1)
        {
            return voxel;
        }

        unsigned half_size = (1 << (max_depth - current_depth)) / 2;
        unsigned index = 0;

        if (location.x >= half_size)
        {
            index |= (1 << 2);
            location.x -= half_size;
        }

        if (location.y >= half_size)
        {
            index |= (1 << 1);
            location.y -= half_size;
        }

        if (location.z >= half_size)
        {
            index |= (1 << 0);
            location.z -= half_size;
        }

        if (!children[index])
        {
            return std::nullopt;
        }

        return children[index]->get(location, current_depth + 1, max_depth);
    }

    std::optional<Voxel> Octree::Node::cast_ray(const Ray &ray, Box box, unsigned current_depth, unsigned max_depth) const
    {
        if (current_depth == max_depth + 1)
        {
            return this->voxel;
        }
        else if (!ray.intersects(box))
        {
            return std::nullopt;
        }

        Box boxes[8];
        Vec3 half_size = box.size() / 2.0f;

        Vec3 offsets[8] = {
            Vec3(0, 0, 0),
            Vec3(0, 1, 0),
            Vec3(1, 0, 1),
            Vec3(1, 1, 0),
            Vec3(0, 0, 1),
            Vec3(1, 0, 0),
            Vec3(0, 1, 1),
            Vec3(1, 1, 1)};

        for (unsigned i = 0; i < 8; ++i)
        {
            Vec3 offset = offsets[i];
            Vec3 min = box.get_min() + offset * half_size;
            Vec3 max = min + half_size;
            boxes[i] = Box(min, max);
        }

        std::optional<Voxel> current_voxel;
        float current_distance = INFINITY;

        for (int i = 0; i < 8; i++)
        {
            if (children[i])
            {
                auto res = children[i]->cast_ray(ray, boxes[i], current_depth + 1, max_depth);

                if (res.has_value() && glm::distance(ray.get_origin(), boxes[i].center()) < current_distance)
                {
                    current_voxel = res;
                    current_distance = glm::length(boxes[i].center());
                }
            }
        }

        return current_voxel;
    }
}