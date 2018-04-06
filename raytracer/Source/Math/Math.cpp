#include "Math.h"

using namespace glm;

vec3 findOthor(const vec3 vec);
{
    if (vec[2] > 0)
    {
        return normalize(vec3(1,0, -vec[0] / vec[2]));
    }
    else if (vec[1] > 0)
    {
        return normalize(vec3(1,-vec[0]/vec[1], 0));
    }
    else if (vec[0] > 0)
    {
        return normalize(vec3(-vec[1]/vec[0], 1, 0));
    }
    return vec3(1,0,0);
}
