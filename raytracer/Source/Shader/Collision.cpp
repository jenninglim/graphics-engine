#include "Collision.h"

bool IntersectBoundingVolume(Ray r,
        BoundingVolume bv);

bool IntersectBoundingVolume(Cone r, BoundingVolume bv);

bool collision(BVH bvh, Ray r, Intersection &closestI)
{
    bool intersectionFound = false;
    if (IntersectBoundingVolume(r, bvh.bv))
    {
        if (bvh.isLeaf == true)
        {
           intersectionFound |= bvh.object->intersection(r, closestI); 
        }
        else
        {
            intersectionFound |= collision(*bvh.left, r, closestI);
            intersectionFound |= collision(*bvh.right, r, closestI);
        }
    }
    else
    {
        return false;
    }
    return intersectionFound;
}

bool collision(BVH bvh, Cone r, Intersection &closestI)
{
    bool intersectionFound = false;
    if (IntersectBoundingVolume(r, bvh.bv))
    {
        if (bvh.isLeaf == true)
        {
           intersectionFound |= bvh.object->intersection(r, closestI); 
        }
        else
        {
            intersectionFound |= collision(*bvh.left, r, closestI);
            intersectionFound |= collision(*bvh.right, r, closestI);
        }
    }
    else
    {
        return false;
    }
    return intersectionFound;
}


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

    if ((tmin[0] > tmax[1]) || (tmin[1] > tmax[0] )) {return false;}
    if (glm::max(tmin[0], tmin[1]) > tmax[2] || tmin[2] > glm::min(tmax[0], tmax[1]))
    {
     return false;
    }
    return true;
}

bool IntersectBoundingVolume(Cone r, BoundingVolume bv)
{
    return true;
}
