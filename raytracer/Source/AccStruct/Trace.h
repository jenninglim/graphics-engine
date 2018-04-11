#ifndef TRACE_H
#define TRACE_H

#include "Octree.h"
#include "glm/ext.hpp"

struct Trace
{
    vec3 colour;
    float occ;
};

float castShadowCone(Octree * root, vec3 point, Light l, float theta);
Trace ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l);

#endif
