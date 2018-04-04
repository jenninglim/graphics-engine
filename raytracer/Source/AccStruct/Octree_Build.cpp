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

Octree::Octree(vector<Object *> objects, BoundingVolume bv, Light l, BVH * bvh)
{
    // Set up root tree.
    this->boxHalfSize = (bv.max - bv.min) / 2.f;
    this->centre = bv.min + boxHalfSize;
    this->bv = bv;
    this->depth = 0;
    this->bvh = bvh;

    this->makeKids(objects, l, bvh); 
}

Octree::Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light l, BVH * bvh)
{
    // Set up root tree.
    this->depth = depth;
    this->boxHalfSize = boxhalfsize;
    this->centre = center;
    this->bv.max = center + boxhalfsize;
    this->bv.min = center - boxhalfsize;
    this->bvh = bvh;

    this->makeKids(objects, l, bvh); 
}

void Octree::makeKids(vector<Object *> objects, Light l, BVH* bvh)
{
    if (toDivide(objects) && depth < OCT_DEPTH)
    {
        this->depth++;
        this->type = NODE;
        for (int i = 0; i < 8; i++)
        {
            this->children[i] = new Octree(objects,
                    offsets[i] * this->boxHalfSize / 2.f + this->centre,
                    this->boxHalfSize/2.f,
                    depth,
                    l,
                    bvh);
            this->directLight += 1.f/8.f * this->children[i]->directLight;
            this->occlusion += 1.f/8.f * this->children[i]->occlusion;
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

        // Calculate occlusion
        Intersection i;
        i.distance = 20;
        vec3 dir = vec3(l.position) - this->centre;
        if (bvh->collision(Ray(vec4(this->centre,0), glm::normalize(dir)), i))
        {
            if (i.distance > glm::l2Norm(dir))
            {
                this->occlusion = 0;
            }
            else
            {
                this->occlusion = 1;
            }
        }
        else
        {
            this->occlusion = 0;
        }
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

bool Octree::collision(Ray r, Intersection &inter)
{
    bool didColide = false;
    if (this->type == LEAF)
    {
        if(IntersectBoundingVolume(r, this->bv))
        {
            float dist = glm::l2Norm(vec3(r.initial), this->centre);
            if (dist < inter.distance)
            {
                inter.position = vec4(this->centre,0);
                inter.colour = this->colour;
                inter.distance = dist;
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
                didColide |= (this->children[i]->collision(r, inter));
            }
        }
    }
    return didColide;
}
