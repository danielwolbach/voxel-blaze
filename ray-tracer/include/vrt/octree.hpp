#pragma once

#include <vrt/common.hpp>
#include <vrt/ray.hpp>
#include <vrt/box.hpp>

namespace vrt
{
    struct Voxel
    {
        Vec3 color;
    };

    class Octree
    {
    public:
        Octree(unsigned depth);
        void insert(const Vec3 &location, const Voxel &voxel);
        void place_random(const unsigned chance);
        std::optional<Voxel> get(const Vec3 &location) const;
        std::optional<Voxel> raycast(const Ray &ray) const;

    private:
        class Node
        {
        public:
            void insert(const Voxel &voxel, Vec3 location, unsigned current_depth, unsigned max_depth);
            std::optional<Voxel> get(Vec3 location, unsigned current_depth, unsigned max_depth) const;
            std::optional<Voxel> cast_ray(const Ray &ray, Box box, unsigned current_depth, unsigned max_depth) const;

        private:
            std::optional<Voxel> voxel;
            std::unique_ptr<Node> children[8];
        };

        Node root;
        unsigned depth;
    };
}