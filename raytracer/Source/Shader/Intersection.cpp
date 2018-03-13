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

vec3 solveLinearEq(Triangle triangle, vec4 start, vec4 dir)
{
    vec4 v0 = triangle.v0;
    vec4 v1 = triangle.v1;
    vec4 v2 = triangle.v2;
    vec3 s = glm::vec3(start.x, start.y, start.z);
    vec3 d = glm::vec3(dir.x, dir.y, dir.z);

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


