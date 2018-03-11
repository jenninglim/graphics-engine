#ifndef CLOSEST_INTERSECTION_H
#define CLOSEST_INTERSECTION_H

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"
#include "Camera.h"

using namespace std;
// Intersection
struct Intersection
{
    glm::vec4 position;
    glm ::vec3 colour;
    float distance;
    vec4 normal;
};

bool IntersectRayBoundingVolume(Ray r,
        BoundingVolume bv);

vec3 solveLinearEq(Triangle triangle, Ray r);


#endif
