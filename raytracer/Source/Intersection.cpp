#include <limits>
#include <glm/glm.hpp>
#include "Intersection.h"
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;
using glm::vec3;
using glm::vec4;

vec3 solveLinearEq(Triangle triangle, Ray r)
{
    vec4 v0 = triangle.v0;
    vec4 v1 = triangle.v1;
    vec4 v2 = triangle.v2;
    vec3 s = glm::vec3(r.initial.x, r.initial.y, r.initial.z);
    vec3 d = glm::vec3(r.direction.x, r.direction.y, r.direction.z);

    vec3 e1 = vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
    vec3 e2 = vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
    vec3 b = vec3(s.x-v0.x,s.y-v0.y,s.z-v0.z);

    mat3 A( -d, e1, e2 );
    //if (determinant(A) != 0)
    //{
        return glm::inverse( A ) * b;
    //}
    //return vec3(0,0,0);
}

bool IntersectRayBoundingVolume(Ray r,
        BoundingVolume bv)
{
    vec3 tmin, tmax;
    for (int i = 0; i < 3; i++)
    {
        if (r.direction[i] >= 0)
        {
            tmin[i] = (bv.min[i] - r.initial[i]) / r.direction[i];
            tmax[i] = (bv.max[i] - r.initial[i]) / r.direction[i];
        }
        else
        {
            tmin[i] = (bv.max[i] - r.initial[i]) / r.direction[i];
            tmax[i] = (bv.min[i] - r.initial[i]) / r.direction[i];
        }
    }

    if ((tmin[0] > tmax[1]) || (tmin[1] > tmax[0] )) {return false;}
    if (glm::max(tmin[0], tmin[1]) > tmax[2] || tmin[2] > glm::min(tmax[0], tmax[1]))
    {
     return false;
    }
    return true;
}

