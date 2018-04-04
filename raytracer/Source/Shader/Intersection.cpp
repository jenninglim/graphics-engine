#include "Intersection.h"
#include "Config.h"

bool IntersectBoundingVolume(Ray r, BoundingVolume bv)
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

    if ((tmin[0] > tmax[1] + EPSILON) || (tmin[1] > tmax[0] + EPSILON )) {return false;}
    if (glm::max(tmin[0], tmin[1]) > tmax[2] -EPSILON || tmin[2] > glm::min(tmax[0], tmax[1]) - EPSILON)
    {
     return false;
    }
    return true;
}

bool IntersectBoundingVolume(Cone r, BoundingVolume bv)
{
    return true;
}
