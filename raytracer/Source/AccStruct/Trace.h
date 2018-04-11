#ifndef TRACE_H
#define TRACE_H

#include "Octree.h"
#include "glm/ext.hpp"

bool insideCube(vec3 p, float e);

struct CloseVox{
    Octree * tree;
    float diff;
};

struct Trace
{
    vec3 colour;
    float occ;
};

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &voxel);
float castShadowCone(Octree * root, vec3 point, Light l, float theta);
Trace ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l);

#endif
