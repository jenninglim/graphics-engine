#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "BVH.h"
#include "Cell.h"

using glm::vec3;

const static float VOXEL_SIZE = glm::pow(2.f,3)/ glm::pow(8.f, OCT_DEPTH);

enum Type {
    LEAF,
    NODE,
    EMPTY,
};

class Octree
{
    public:
        Type type;
        Octree * children;
        Octree * parent;
        vec3 centre;
        vec3 boxHalfSize;

        Cell * brick;
        Cell * voxel;

    public:
        bool toDivide(vector<Object *> objects, vec3 &colour);
        void makeKids(vector<Object *> objects, Light l, BVH * bvh, int depth);
        Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light light, BVH * bvh, Octree * parent);
        void makeTexture(const vec3 colour);
        void updateTexture(int dept);
        void mipmap();
        void BrickEdgeCopy();
        void PrintBrick();
        void LeafEdgeCopy();
        float interOcc(vec3 point);
        vec3 interCol(vec3 point);
        void AverageBrick();

        Octree();
        Octree(vector<Object *> objects, BoundingVolume bv, Light light, BVH * bvh);
        bool collision(Ray r, Intersection &inter, int d_depth, int c_depth);
};
#endif
