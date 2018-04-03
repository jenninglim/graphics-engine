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
}

Octree::Octree(vector<Object *> objects, BoundingVolume bv, Light l)
{
    // Set up root tree.
    this->boxHalfSize = (bv.max - bv.min) / 2.f;
    this->centre = bv.min + boxHalfSize;
    this->bv = bv;
    this->depth = 0;
    this->makeKids(objects, l); 
}

Octree::Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light l)
{
    // Set up root tree.
    this->depth = depth;
    this->boxHalfSize = boxhalfsize;
    this->centre = center;
    this->bv.max = center + boxhalfsize;
    this->bv.min = center - boxhalfsize;
    this->makeKids(objects, l); 
}

void Octree::makeKids(vector<Object *> objects, Light l)
{
    if (toDivide(objects) && depth < 7)
    {
        this->depth++;
        this->type = NODE;
        for (int i = 0; i < 8; i++)
        {
            this->children[i] = new Octree(objects,
                    offsets[i] * this->boxHalfSize / 2.f + this->centre,
                    this->boxHalfSize/2.f,
                    depth,
                    l);
            this->directLight += 1.f/8.f * this->children[i]->directLight;
        }
    }
    else if (!toDivide(objects))
    {
        this->type = EMPTY;
    }
    else
    {
        this->type = LEAF;
        this->directLight = DirectLight(vec4(this->centre,0), this->normal, l);
    }
}

bool Octree::toDivide(vector<Object *> objects)
{
    Intersection inter;
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->boxOverlap(this->centre, this->boxHalfSize, inter))
        {
            this->normal = vec3(inter.normal);
            this->colour = inter.colour;
            return true;
        }
    }
    if (LESSTHAN(this->boxHalfSize,SCALING))
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
            colour = this->colour * this->directLight;
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
