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

struct tex_t {
    vec3 * brick;
    vec3 colour;
    float occ;
};

struct Trace
{
    vec3 colour;
    float occ;
};

class Octree
{
    public:
        Type type;
        Octree * children;
        vec3 centre;
        vec3 boxHalfSize;
        
        tex_t * voxel;

        bool toDivide(vector<Object *> objects, vec3 &colour);
        void makeKids(vector<Object *> objects, Light l, BVH * bvh, int depth);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light light, BVH * bvh);
        void makeTexture(const vec3 colour);
        void updateTexture();
        void mipmap();
        void BrickEdgeCopy();
        void PrintBrick();
        float interOcc(vec3 point);
        vec3 interCol(vec3 point);

    public:
        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv, Light light, BVH * bvh);
        bool collision(Ray r, Intersection &inter);
};

float castShadowCone(Octree * root, vec3 point, Light l, float theta);
Trace ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l);

#endif
