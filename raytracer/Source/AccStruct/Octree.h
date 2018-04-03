#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"

#define SCALING 5.f

using glm::vec3;

enum Type {
    LEAF,
    NODE,
    EMPTY,
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
        BoundingVolume bv;
        Data data;
        bool toDivide(vector<Object *> objects);
        void makeKids(vector<Object *> objects, Light l);
        int depth;
        vec3 colour;
        vec3 normal;
        vec3 directLight;

    public:
        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv, Light light);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light light);
        bool collision(Ray r, vec3& colour);

};

#endif
