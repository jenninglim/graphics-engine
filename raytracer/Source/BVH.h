#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include "Object.h"
#include "Ray.h"
#include "Intersection.h"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

class BVH
{
    public:
        BVH* left;
        BVH* right;
        bool isLeaf;
        Object object;
        BoundingVolume bv;
        BVH();
        BVH(vector<Object> objects);
};

bool collision(BVH bvh, Ray r, Intersection &i);

#endif
