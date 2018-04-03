#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <vector>

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
        Octree * childern[8];
        BoundingVolume bv;
        Data data;

    public:
        Octree();
        Octree(vector<Object *> objects);
};

#endif
