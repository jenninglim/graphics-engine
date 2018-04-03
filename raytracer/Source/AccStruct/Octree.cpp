#include "Octree.h"
#include "Config.h"

#ifdef DEBUG
    #include <iostream>
    #include <glm/ext.hpp>
#endif

#define LESSTHAN(v1,scale) \
    (v1[0] * scale < 1.f + EPSILON && \
            v1[1] * scale < 1.f + EPSILON && \
            v1[2] * scale < 1.f + EPSILON) 

const static vec3 offsets[8] = { vec3(1,1,1),
                            vec3(-1,1,1),
                            vec3(1,-1,1),
                            vec3(-1,-1,1),
                            vec3(1,1,-1),
                            vec3(-1,1,-1),
                            vec3(1,-1,-1),
                            vec3(-1,-1,-1)};

Octree::Octree()
{
    this->scaling = SCREEN_WIDTH * SCREEN_WIDTH * SCREEN_WIDTH;
}

Octree::Octree(vector<Object *> objects, BoundingVolume bv)
{
    // Set up root tree.
    this->boxHalfSize = (bv.max - bv.min) / 2.f;
    this->centre = bv.min + boxHalfSize;
    this->bv = bv;
    this->depth = 0;
    this->scaling = SCREEN_WIDTH * SCREEN_WIDTH * SCREEN_WIDTH;
    cout << "box half size: " << glm::to_string(bv.max) << endl;

    this->makeKids(objects); 
}

Octree::Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth)
{
    // Set up root tree.
    this->depth = depth;
    this->boxHalfSize = boxhalfsize;
    this->centre = center;
    this->bv.max = center + boxhalfsize;
    this->bv.min = center - boxhalfsize;
    this->scaling = SCREEN_WIDTH * SCREEN_WIDTH * SCREEN_WIDTH;
    this->makeKids(objects); 
    cout << this->depth << "centre: " << glm::to_string(this->centre) << endl;
    cout << this->depth << "bvmax: " << glm::to_string(this->bv.max) << endl;
    cout << this->depth << "bvmin: " << glm::to_string(this->bv.min) << endl;
}

void Octree::makeKids(vector<Object *> objects)
{
    if (toDivide(objects) && depth < 5)
    {
        this->depth++;
        this->type = NODE;
        for (int i = 0; i < 8; i++)
        {
            this->children[i] = new Octree(objects,
                    offsets[i] * this->boxHalfSize / 2.f + this->centre,
                    this->boxHalfSize/2.f,
                    depth);
        }
    }
    else
    {
        this->type = LEAF;
    }
}

bool Octree::toDivide(vector<Object *> objects)
{

    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->boxOverlap(this->centre, this->boxHalfSize, this->colour))
        {
            return true;
        }
    }
    if (LESSTHAN(this->boxHalfSize,this->scaling))
    {
        return false;
    }
    return false;
}

bool Octree::collision(Ray r, vec3 & colour)
{
    bool didColide = false;
    if (this->type == LEAF)
    {
        if(IntersectBoundingVolume(r, this->bv))
        {
            if (!(this->colour[0] < EPSILON && this->colour[1] < EPSILON && this->colour[2] < EPSILON))
            {
                colour = this->colour;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    if (this->type == NODE)
    {
        if (IntersectBoundingVolume(r, this->bv))
        {
            for (int i = 0; i < 8; i ++)
            {
                didColide |= (this->children[i]->collision(r, colour));
            }
        }
    }
    return didColide;
}
