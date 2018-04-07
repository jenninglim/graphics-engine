#include "Collision.h"

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



