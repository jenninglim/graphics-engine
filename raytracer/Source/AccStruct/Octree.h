#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "BVH.h"

#define SCALING 5.f

using glm::vec3;

enum Type {
    LEAF,
    NODE,
    EMPTY,
};

struct Amb_t {
    vec3 colour;
    float occ;
};

struct Trace
{
    vec3 colour;
    float occlusion;
};

class Octree
{
    public:
        Type type;
        Octree * children;
        vec3 centre;
        vec3 boxHalfSize;
        
        vec3 colour;
        vec3 directLight;
        float occlusion;

        bool toDivide(vector<Object *> objects, Light l);
        void makeKids(vector<Object *> objects, Light l, BVH * bvh, int depth);

    public:
        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv, Light light, BVH * bvh);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light light, BVH * bvh);
        bool collision(Ray r, Intersection &inter);
};

float castShadowCone(Octree * root, vec3 point, Light l, float theta);
Amb_t ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l);

#endif
