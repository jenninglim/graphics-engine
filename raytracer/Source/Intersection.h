#ifndef CLOSEST_INTERSECTION_H
#define CLOSEST_INTERSECTION_H

#include <glm/glm.hpp>
#include <vector>
#include "Triangle.h"
#include "Camera.h"

using namespace std;
// Intersection
struct Intersection
{
    glm::vec4 position;
    float distance;
    int triangleIndex;
};

bool ClosestIntersection(vec4 start,
        vec4 dir,
        const vector<Triangle> &triangles,
        Intersection &closestIntersection);
#endif
