#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>

using glm::vec3;

enum Type {
    LEAF,
    NODE,
};

struct Data {
    vec3 color;
};

class Octree
{
    private:
        Type type;
        Octree * children[8];
        vec3 centre;
        vec3 boxHalfSize;
        float scaling;
        BoundingVolume bv;
        Data data;
        bool toDivide(vector<Object *> objects);
        void makeKids(vector<Object *> objects);
        int depth;
        vec3 colour;

    public:
        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth);
        bool collision(Ray r, vec3& colour);

};

#endif
