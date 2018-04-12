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

const static int connectOffset[8][3] = {{1,2,4}, // 0
                                         {0,3,5}, // 1
                                         {3,0,6}, // 2
                                         {2,1,7}, // 3
                                         {5,6,0}, // 4
                                         {4,7,1}, // 5
                                         {7,4,2}, // 6
                                         {6,5,3}};// 7


static vector<Object *> * m_objects;

static BVH * m_bvh;

void ConnectKid(Octree * kid1, Octree * kid2, int orient);
void ConnectNodes(Octree * t1, Octree * t2 );

Octree::Octree()
{
}

Octree::Octree(vector<Object *> objects, BoundingVolume bv, Light l, BVH * bvh)
{
    m_objects =&objects;
    m_bvh = bvh;
    int depth =0;

    // Set up root tree.
    this->boxHalfSize = (bv.max - bv.min) / 2.f * (1 + 2 * EPSILON);
    this->centre = bv.min + boxHalfSize;
    this->voxel = NULL;
    this->brick = NULL;
    this->AssignType();
    for (int i = 0; i< NEIGHBOURS; i++)
    {
        this->neighbours[i] = NULL;
    }
    
    queue<Octree *> q;
    q.push(this);

    Octree * current;
    while (!q.empty())
    {
        // Change current octree
        current = q.front();
        q.pop();

        // Make Kids
        // Assigns Center, HalfSize, and Type
        current->makeKids(depth, q);

        // Connect children iff it is a node (it has no children otherwise).
        if (current->type == NODE)
        {
            current->connectKids();
        }

        for (int i = 0; i < NEIGHBOURS; i++)
        {
            if (current->neighbours[i] != NULL
                    && current->type == NODE
                    && current->neighbours[i]->type == NODE)
            {
                ConnectNodes(current, current->neighbours[i]);
            }
        }

    }
}

void Octree::makeKids(int depth, queue<Octree *> &q)
{
    if (this->type == NODE)
    {
        // Assign centers and size and push onto stack
        for (int i = 0; i < 8; i++)
        {
            // Assign centres and halfsizes
            this->children[i].centre = offsets[i]
                * this->boxHalfSize/2.f;
            this->children[i].boxHalfSize =  this->boxHalfSize /2.f;
            this->children[i].AssignType();

            for (int j = 0; j < NEIGHBOURS; j ++)
            {
                this->children[i].neighbours[j] = NULL;
            }

            if (this->children[i].type == NODE)
            {
                q.push(&this->children[i]);
            }
        }
    }
}

// Connects Two Octrees Children
void ConnectNodes( Octree * t1, Octree * t2)
{
    int index, orient, orientoffset;
    if (t1 != t2)
    {
        orientoffset = 1;
    }
    else
    {
        orientoffset = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            index = connectOffset[i][j];
            if (j == 0) // 2/3
            {
                orient = 2 + (orientoffset+ i % 2) % 2;
            }
            else if (j == 1) // 0/1
            {
                orient = 1 - (orientoffset + int(i / 2) % 2) %2;
            }
            else if (j == 2) // 4/5
            {
                orient = 5 - (orientoffset + (int) floor(i / 4)) % 2;
            }

            ConnectKid(&t1->children[i], &t2->children[index], orient);
        }
    }
}

// Out: LEAF, EMPTY or NODE
// In: Centre and HalfBox
void Octree::AssignType()
{
    float size;
    vec3 colour;
    if (this->toDivide(colour))
    {
        this->makeTexture(colour);
        size = l2Norm(this->boxHalfSize);
        if (size - VOXEL_SIZE > EPSILON)
        {
            this->type = NODE;
            this->children = new Octree[8];
        }
        else
        {
            this->type = LEAF;
        }
    }
    else
    {
        this->type = EMPTY;
    }
}
// Connect is good.
// Assumes Octree is typed.
void Octree::connectKids()
{
    ConnectNodes(this, this);
}

void ConnectKid(Octree * kid1, Octree * kid2, int orient)
{
    kid1->neighbours[orient] = kid2;
}

bool Octree::toDivide(vec3 & colour)
{
    Intersection inter;
    for (int i = 0; i < m_objects->size(); i++)
    {
        if (m_objects->at(i)->boxOverlap(this->centre, this->boxHalfSize, inter))
        {
            colour = inter.colour;
            return true;
        }
    }
    return false;
}

bool Octree::collision(Ray r, Intersection &inter, int d_depth, int c_depth)
{
    bool didColide = false;
    vec3 min, max;
    max = this->centre + this->boxHalfSize;
    min = this->centre - this->boxHalfSize;
    float dist = 0;

    if (d_depth == c_depth && this->type != EMPTY)
    {
        max = this->centre + this->boxHalfSize;
        min = this->centre - this->boxHalfSize;
        if(IntersectBoundingVolume(r, BoundingVolume(min,max)))
        {
            dist = glm::l2Norm(vec3(r.initial), this->centre);
            if (dist < inter.distance)
            {
                inter.position = vec4(this->centre,0);
                inter.colour = vec3(this->voxel->occ);
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
                didColide |= (this->children[i].collision(r, inter, d_depth, c_depth+1));
            }
        }
    }
    return didColide;
}
