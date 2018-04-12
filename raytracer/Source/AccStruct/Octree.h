#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "BVH.h"
#include "Cell.h"
#include <queue>

#define NEIGHBOURS 6

using glm::vec3;

const static float VOXEL_SIZE = 1/ glm::pow(2.f, OCT_DEPTH);

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
        Octree * neighbours[6];
        vec3 centre;
        vec3 boxHalfSize;

        Cell * brick;
        Cell * voxel;

    public:
        // Building Octree Ops
        bool toDivide(vec3 &colour);
        void AssignType();
        void connectKids();
        void makeKids(int depth, queue<Octree *> &q);

        void makeTexture(const vec3 colour);
        void updateTexture(Light l);
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
