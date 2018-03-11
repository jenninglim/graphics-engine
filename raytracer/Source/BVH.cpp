#include "BVH.h"

using namespace std;
void computeBoundingVolume(vector<Object> objects);

BVH::BVH()
{
    left = NULL;
    right = NULL;
}

BVH::BVH(vector<Object> objects)
{
    computeBoundingVolume(objects);
    if (objects.size() == 1)
    {
        
    }
    else
    {

    }
}

void computeBoundingVolume(vector<Object> objects)
{
    vec3 max = vec3(std::numeric_limits<float>::min());
    vec3 min = vec3(std::numeric_limits<float>::max());

    for (int i = 0; i < objects.size(); i ++)
    {
        if (objects[i].bv.min.x < min.x) { min.x=objects[i].bv.min.x; }
        if (objects[i].bv.min.y < min.y) { min.y=objects[i].bv.min.y; }
        if (objects[i].bv.min.z < min.z) { min.z=objects[i].bv.min.z; }
        if (objects[i].bv.max.x < max.x) { max.x=objects[i].bv.max.x; }
        if (objects[i].bv.max.y < max.y) { max.y=objects[i].bv.max.y; }
        if (objects[i].bv.max.z < max.z) { max.z=objects[i].bv.max.z; }
    }
}
