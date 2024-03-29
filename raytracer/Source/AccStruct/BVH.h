#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include "Object.h"
#include "BoundingVolume.h"
#include "Intersection.h"
#include <vector>

class Object;

class BVH
{
    public:
        BVH* left;
        BVH* right;
        bool isLeaf;
        Object * object;
        BoundingVolume bv;
        BVH();
        BVH(std::vector<Object *> objects);
        bool collision(Ray r, Intersection &closestI);
};

#endif
