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
    this->boxHalfSize = (bv.max - bv.min) / 2.f  * (1+EPSILON);
    this->centre = bv.min + boxHalfSize;
    this->makeKids(objects, l, bvh, 0.); 
}

Octree::Octree(vector<Object *> objects, vec3 center, vec3 boxhalfsize, int depth, Light l, BVH * bvh)
{
    // Set up root tree.
    this->boxHalfSize = boxhalfsize * (1 + EPSILON);
    this->centre = center;
    this->makeKids(objects, l, bvh, depth); 
}

static const int cubeOffset[4] = {1,1,2,2};
void Octree::updateTexture()
{
    if (this->type == NODE)
    {
        this->voxel->brick = new vec3[3 * 3 * 3];
        for (int i = 0; i < 3 * 3 *3; i++)
        {
            this->voxel->brick[i] = vec3(0);
        }
        int index = 0;
        for (int i = 0; i < 8; i ++)
        {
            index = (i % 4) + cubeOffset[i % 4] + 9*floor(i / 4);
            if (this->children[i].type != EMPTY)
            {
                this->voxel->brick[index] = children[i].voxel->colour;
            }
            if (this->children[i].type == NODE)
            {

            }
        }
        this->mipmap();
    }
}

void Octree::makeTexture(const vec3 colour)
{
    this->voxel = new tex_t();
    this->voxel->occ = 0;
    if (this->type == LEAF)
    {
        this->voxel->colour = colour;
    }
}

static const int mipmapoffset[5][3] = {{0,2,4},
                                {6,6,6},
                                {18,18,18},
                                {1,-3,-9},
                                {9,9,3}}; // 
void Octree::mipmap()
{
    int index;
/*
    for (int i= 0; i < 3; i ++)
    {
        for (int j = 0; j <3; j++)
        {
            for (int k=0; k<3; k++)
            {
                cout << to_string(this->voxel->brick[i*9+j*3+k]);
            }
            cout << endl;
        }
        cout << endl;
    }
    */
    for (int i = 0; i < 3; i++) // X Y Z PASS
    {
        for (int j = 0; j < 3; j++) // CUBE ASSIGMENT;
        {
            for (int k = 0; k < 3; k++) 
            {
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i]; // edge assignment
                this->voxel->brick[index] = this->voxel->brick[index + mipmapoffset[3][i]];
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i] + mipmapoffset[3][i]; // edge assignment
                this->voxel->brick[index] += this->voxel->brick[index + mipmapoffset[3][i]];
                this->voxel->brick[index] /= 2;
            }
        }
   }

    this->voxel->colour = this->voxel->brick[14];

}

void Octree::makeKids(vector<Object *> objects, Light l, BVH* bvh, int depth)
{
    vec3 colour;
    if (this->toDivide(objects, colour))
    {
        if (depth < OCT_DEPTH)
        {
            this->type = NODE;
            makeTexture(colour);

            this->children = new Octree[8];
            for (int i = 0; i < 8; i++)
            {
                this->children[i] = Octree(objects,
                        offsets[i] * this->boxHalfSize / 2.f + this->centre,
                        this->boxHalfSize/2.f,
                        depth+1,
                        l,
                        bvh);
            }
            this->updateTexture();
        }
        else
        {
            this->type = LEAF;
            makeTexture(colour);
            // Calculate occlusion
            Intersection i;
            i.distance = 20;
            vec3 dir = vec3(l.position) - this->centre;
            if (bvh->collision(Ray(vec4(this->centre,0), glm::normalize(dir)), i))
            {
                if (i.distance > glm::l2Norm(dir) && i.distance > EPSILON)
                {
                    this->voxel->occ = 0;
                }
                else
                {
                    this->voxel->occ = 1;
                }
            }
            else
            {
                this->voxel->occ = 0;
            }
        }

    }
    else 
    {
        this->type = EMPTY;
    }
}

bool Octree::toDivide(vector<Object *> objects, vec3 & colour)
{
    Intersection inter;
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->boxOverlap(this->centre, this->boxHalfSize, inter))
        {
            colour = inter.colour;
            return true;
        }
    }
    return false;
}

bool Octree::collision(Ray r, Intersection &inter)
{
    bool didColide = false;
    vec3 min, max;
    max = this->centre + this->boxHalfSize;
    min = this->centre - this->boxHalfSize;
    float dist = 0;

    if (this->type == LEAF)
    {
        max = this->centre + this->boxHalfSize;
        min = this->centre - this->boxHalfSize;
        if(IntersectBoundingVolume(r, BoundingVolume(min,max)))
        {
            dist = glm::l2Norm(vec3(r.initial), this->centre);
            if (dist < inter.distance)
            {
                inter.position = vec4(this->centre,0);
                inter.colour = vec3(this->voxel->colour[0]);
                inter.distance = dist;
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else if (this->type == EMPTY)
    {
        return false;
    }
    else if (this->type == NODE)
    {
        if (IntersectBoundingVolume(r, BoundingVolume(min,max)))
        {
            for (int i = 0; i < 8; i ++)
            {
                didColide |= (this->children[i].collision(r, inter));
            }
        }
    }
    return didColide;
}
