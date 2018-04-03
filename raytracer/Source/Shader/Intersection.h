#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include "Ray.h"
#include "BoundingVolume.h"

using namespace glm;

struct Intersection
{
    glm::vec4 position;
    glm ::vec3 colour;
    float distance;
    glm::vec4 normal;
    float reflect_ratio;
    float refract_ratio;
    float ior;
    float area;
};

bool IntersectBoundingVolume(Ray r,
        BoundingVolume bv);

bool IntersectBoundingVolume(Cone r, BoundingVolume bv);


#endif
