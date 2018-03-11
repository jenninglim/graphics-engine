#include "BVH.h"
#include <assert.h>

using namespace std;

BoundingVolume computeBoundingVolume(vector<Object> objects);
vector<vector<Object> > partitionObject(vector<Object> objects);

BVH::BVH()
{
    left = NULL;
    right = NULL;
}

BVH::BVH(vector<Object> objects)
{
    assert(objects.size() > 0);
    bv = computeBoundingVolume(objects);
    
    if (objects.size() == 1)
    {
        isLeaf = true;
        object = objects[0];
    }
    else
    {
        isLeaf = false;
        vector<vector<Object> > partitioning = partitionObject(objects);        
        left = new BVH(objects);
        right = new BVH(objects);
    }
}

bool collision(BVH bvh, Ray r, Intersection &closestI)
{
    vec4 start = r.initial;
    vec4 dir = r.direction;
    bool intersectionFound = false;
    if (IntersectRayBoundingVolume(r, bvh.bv))
    {
        if (bvh.isLeaf == true)
        {
            for (int i=0; i < bvh.object.triangles.size(); i++)
            {
                vec3 x_value = solveLinearEq(bvh.object.triangles[i],r);
                if(x_value.y >= 0
                        && x_value.z >= 0
                        && x_value.y + x_value.z <= 1
                        && x_value.x > EPSILON)
                {
                    //Valid Intersection found
                    intersectionFound = true;
                    if(x_value.x < closestI.distance){
                        closestI.position = start + x_value.x * dir;
                        closestI.distance = x_value.x;
                        closestI.colour = bvh.object.triangles[i].color;
                        closestI.normal = bvh.object.triangles[i].normal;

                    }
                }
            }
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

vector<vector<Object> > partitionObject(vector<Object> objects)
{
    // Naive median parititoning.
    //
    int index = objects.size()/2;
    vector<Object> left, right;
    vector<vector<Object> >  result = vector<vector<Object> >();
    for (int i = 0; i < index; i++)
    {
        left.push_back(objects[i]);
    }
    for (int i = index; i < objects.size(); i++)
    {
        right.push_back(objects[i]);
    }
    result.push_back(left);
    result.push_back(right);
    return result;
}


BoundingVolume computeBoundingVolume(const vector<Object> objects)
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
    return BoundingVolume(min,max);
}
