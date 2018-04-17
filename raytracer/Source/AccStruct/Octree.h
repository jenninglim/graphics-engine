#ifndef OCTREE_H
#define OCTREE_H

#include "Object.h"
#include "BoundingVolume.h"
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "BVH.h"
#include "Voxel.h"
#include <queue>

#define NEIGHBOURS 6

using glm::vec3;

const static float VOXEL_SIZE = 1/ glm::pow(2.f, OCT_DEPTH);

static vector<Object *> * m_objects;

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

        Voxel * brick;
        Voxel * voxel;

    public:
        // Building Octree Ops
        bool toDivide(Intersection &inter);
        void AssignType();
        void connectKids();
        void makeKids(int depth, queue<Octree *> &q);

        void makeTexture(const Intersection inter);
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

void updateTextureOctree(Octree * tree, Light l, BVH * bvh);

#endif
