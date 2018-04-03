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
    int dummy;
};

class Octree
{
    private:
        Type type;
        Octree * children[8];
        vec3 centre;
        vec3 boxHalfSize;
        vec3 minVoxelSize;
        Data data;
        bool toDivide(vector<Object *> objects);
        void makeKids(vector<Object *> objects);

    public:
        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize);

};

#endif
