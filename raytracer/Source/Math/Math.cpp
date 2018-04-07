#include "MyMath.h"

using namespace glm;

vec3 findOthor(const vec3 v1, const vec3 v2)
{
    return normalize(cross(v1,v2));
}

mat3 projMatr(const vec3 e1, const vec3 e2, const vec3 e3)
{
   return glm::inverse(mat3(e1,e2,e3)); 
}

vec3 findOthor(const vec3 vec)
{
    if (abs(vec[2]) > 0)
    {
        return normalize(vec3(1,0, -vec[0] / vec[2]));
    }
    else if (abs(vec[1]) > 0)
    {
        return normalize(vec3(1,-vec[0]/vec[1], 0));
    }
    else if (abs(vec[0]) > 0)
    {
        return normalize(vec3(-vec[1]/vec[0], 1, 0));
    }
    return vec3(1,0,0);
}
