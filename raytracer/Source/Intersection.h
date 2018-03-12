#ifndef CLOSEST_INTERSECTION_H
#define CLOSEST_INTERSECTION_H

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

using namespace std;
// Intersection
struct Intersection
{
    glm::vec4 position;
    glm ::vec3 colour;
    float distance;
    vec4 normal;
};

vec3 solveLinearEq(Triangle triangle, vec4 start, vec4 dir);

#endif
