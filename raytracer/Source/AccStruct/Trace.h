#ifndef TRACE_H
#define TRACE_H

#define SHADOW_CONE 0.325
#define AMBIEN_CONE 0.325
#include "Octree.h"
#include "glm/ext.hpp"

bool insideCube(vec3 p, float e);

struct CloseVox{
    Octree * tree;
    Octree * tree2;
};

struct Trace
{
    vec3 col;
    float occ;
    Trace();
};

Trace operator+(const Trace c1, const Trace c2);
    //Trace operator+(const Trace c1, const Cell &c);

bool getVoxel(Octree * root, const vec3 point, const int depth, CloseVox &vox);
float castShadowCone(Octree * root, vec3 point, Light l, float theta);
Trace ambientOcclusion(Octree * root, vec3 point1, vec3 normal);
float castShadowCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist);
Cell averageNeighVoxel(Octree * root);

#endif
