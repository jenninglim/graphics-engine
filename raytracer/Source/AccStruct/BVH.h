#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include "Object.h"
#include "Intersection.h"
#include "Ray.h"

class BVH
{
    public:
        BVH* left;
        BVH* right;
        bool isLeaf;
        Object * object;
        BoundingVolume bv;
        BVH();
        BVH(vector<Object*> objects);
};

bool collision(BVH bvh, Ray r, Intersection &i);

#endif
