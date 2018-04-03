#include "Octree.h"
#include "Config.h"

#define LESSTHAN(v1,v2) \
    (v1[0] < v2[0] && \
            v1[1] < v2[0] && \
            v1[2] < v2[2]) 

const static vec3 offsets[8] = { vec3(1,1,1),
                            vec3(-1,1,1),
                            vec3(1,-1,1),
                            vec3(-1,-1,1),
                            vec3(1,1,-1),
                            vec3(-1,1,-1),
                            vec3(-1,1,-1),
                            vec3(1,-1,-1)};

Octree::Octree()
{
    this->minVoxelSize = vec3(1/ SCREEN_WIDTH * SCREEN_WIDTH * SCREEN_WIDTH);
}

Octree::Octree(vector<Object *> objects, BoundingVolume bv)
{
    // Set up root tree.
    this->boxHalfSize = (bv.max - bv.min) / 2.f;
    this->centre = bv.min + boxHalfSize;
    this->bv = bv;
    this->makeKids(objects); 
}

Octree::Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize)
{
    // Set up root tree.
    this->boxHalfSize = boxhalfsize;
    this->centre = center;
    this->bv.max = center + boxhalfsize;
    this->bv.min = center - boxhalfsize;
    this->makeKids(objects); 
}

void Octree::makeKids(vector<Object *> objects)
{
     
    if (toDivide(objects))
    {
        this->type = NODE;
        for (int i = 0; i < 8; i++)
        {
            this->children[i] = new Octree(objects, offsets[i]* this->boxHalfSize + this->centre, this->boxHalfSize/2.f);
        }
    }
    else
    {
        this->type = LEAF;
    }
}

bool Octree::toDivide(vector<Object *> objects)
{
    if (LESSTHAN(this->boxHalfSize,this->minVoxelSize))
    {
        return false;
    }
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->boxOverlap(this->centre, this->boxHalfSize))
        {
            return true;
        }
    }
    return false;
}

bool Octree::collision(Ray r)
{
    bool collision = false;
    if (this->type == LEAF)
    {
        return IntersectBoundingVolume(r, this->bv);
    }
    if (this->type == NODE)
    {
        if (IntersectBoundingVolume(r, this->bv))
        {
            for (int i = 0; i < 8; i ++)
            {
                collision |= this->children[i]->collision(r);
            }
        }
    }
    return collision;
}
