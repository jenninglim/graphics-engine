#include "Octree.h"
#include <csignal>
#include "glm/ext.hpp"
#include <stack>

using namespace glm;

static const int mipmapoffset[5][3] = {{0,2,4},
                                {6,6,6},
                                {18,18,18},
                                {1,-3,-9},
                                {9,9,3}}; // 

static const int cubeOffset[4] = {1,1,2,2};

const static int weightoffset[8][4] = {{26,25,23,17},
                                {24,25,21,15},
                                {20,19,23,11},
                                {18,19,21,9},
                                {8,7,5,18},
                                {6,7,3,15},
                                {2,1,5,11},
                                {0,1,3,9}};

const static int idenoffset[8][3] = {{1,2,4},
                                      {-1,2,4},
                                      {1,-2,4},
                                      {-1,-2,4},
                                      {1,2,-4},
                                      {-1,2,-4},
                                      {1,-2,-4},
                                      {-1,-2,-4}};

void EdgeCopy(Voxel * brick1, Voxel * brick2, int orient);
void ChildrenEdgeCopy(Octree * t, int c1, int c2);

void updateTextureOctree(Octree * tree, Light l, BVH * bvh)
{
    queue<Octree *> que;
    stack<Octree *> sta;
    Intersection inter;

    que.push(tree);
    vec3 norm;
    
    Octree * current;
    Octree * node; // TODO: remove this
    while (!que.empty())
    {
        current = que.front();
        sta.push(current);
        que.pop();

        for (int i = 0; i < 8; i++)
        {
            node = &current->children[i];
            if (current->children[i].type == NODE)
            {
                que.push(&current->children[i]);
            }
            else if (current->children[i].type == LEAF)
            {
                // Shadow Filling
                vec3 distance = vec3(l.position) - node->centre;
                inter.distance = l2Norm(distance) - 0.05;
                vec3 dir = normalize(distance);
                if (bvh->collision(Ray(node->centre + dir*(0.05f), glm::normalize(dir)), inter))
                {
                    node->voxel->occ = 1;
                }
                else
                {
                    node->voxel->occ = 0;
                }     

                // Radiance filling
                norm = node->voxel->getNorm();
                node->voxel->irrad = DirectLight( vec4(node->centre, 0),
                        norm,
                        l);
                                            
            }
        }
    }

    while(!sta.empty())
    {
        // Current is a node
        current = sta.top();
        sta.pop();

        int index = 0;
        int acc=0;
        int other=0;

        for (int i = 0; i < 3 * 3 *3; i++)
        {
            current->brick[i] = Voxel();
        }
        *current->voxel = Voxel();

        // Blur Nodes
        current->BrickEdgeCopy();
        for (int i = 0; i < 8; i ++)
        {
            index = (i % 4) + cubeOffset[i % 4] + 9*floor(i / 4);
            if (current->children[i].type != EMPTY)
            {
                current->brick[index] = *current->children[i].voxel;
            }
            else
            {
                acc = 0;
                other= 0;
                //compute Identity
                for (int j = 0; j < 8; j++)
                {
                    other = j;//+idenoffset[i][j];
                    if (current->children[other].type != EMPTY)
                    {
                        current->brick[index] = current->brick[index]+*current->children[other].voxel;//(float) (OCT_DEPTH-depth+1);
                        acc++;
                    }
                }
                if(acc>0)
                {
                    current->brick[index] = current->brick[index] / (float) acc;
                }
            }
        }
        current->mipmap();
        current->AverageBrick();
    }
}


void Octree::makeTexture(const Intersection inter)
{
    if (this->type != EMPTY)
    {
        if (this->type == LEAF)
        {
            this->voxel = new VoxelLeaf(inter);
        }
        else if (this->type == NODE)
        {
            this->voxel = new Voxel();
            this->brick = new Voxel[3 * 3 * 3];
        }
    }
}

void Octree::mipmap()
{
    int index;
    for (int i = 0; i < 3; i++) // X Y Z PASS
    {
        for (int j = 0; j < 3; j++) // CUBE ASSIGMENT;
        {
            for (int k = 0; k < 3; k++) 
            {
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i]; // edge assignment
                this->brick[index] = this->brick[index + mipmapoffset[3][i]];
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i] + mipmapoffset[3][i]; // edge assignment
                this->brick[index] = this->brick[index]+this->brick[index + mipmapoffset[3][i]];
                this->brick[index] = this->brick[index]/ 2;
            }
        }
    }
}

void Octree::AverageBrick()
{
    Voxel cell = Voxel();
    int count =0;

    if (this->type == NODE)
    {
        *this->voxel = cell;
        for (int i = 0; i < 8; i++)
        {
            if (this->children[i].type != EMPTY)
            {
                *this->voxel = *this->voxel + this->brick[i];
                count ++;
            }
        }
        if (count >0)
        {
            *this->voxel = *this->voxel/ (float) count;
        }
    }
}
void Octree::BrickEdgeCopy()
{    
    for (int i = 0; i < 8; i++)
    {
        if (this->children[i].neighbours[0] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[0]->type == NODE)
            {
                EdgeCopy(this->children[i].brick,
                        this->children[i].neighbours[0]->brick,
                        0);
                this->children[i].AverageBrick();
                this->children[i].neighbours[0]->AverageBrick();
            }  
        }
        if (this->children[i].neighbours[1] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[1]->type == NODE)
            {
                EdgeCopy(this->children[i].neighbours[1]->brick,
                        this->children[i].brick,
                        0);
                this->children[i].AverageBrick();
                this->children[i].neighbours[1]->AverageBrick();
            }
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (this->children[i].neighbours[3] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[3]->type == NODE)
            {
                EdgeCopy(this->children[i].neighbours[3]->brick,
                        this->children[i].brick,
                        1);
                this->children[i].AverageBrick();
                this->children[i].neighbours[3]->AverageBrick();
            }
        }
        if (this->children[i].neighbours[2] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[2]->type == NODE)
            {
                EdgeCopy(this->children[i].brick,
                        this->children[i].neighbours[2]->brick,
                        1);
                this->children[i].AverageBrick();
                this->children[i].neighbours[2]->AverageBrick();
            }
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (this->children[i].neighbours[5] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[5]->type == NODE)
            {
                EdgeCopy(this->children[i].neighbours[5]->brick,
                        this->children[i].brick,
                        2);
                this->children[i].AverageBrick();
                this->children[i].neighbours[5]->AverageBrick();
            }  
        }
        if (this->children[i].neighbours[4] != NULL)
        {
            if (this->children[i].type == NODE &&
                    this->children[i].neighbours[4]->type == NODE)
            {
                EdgeCopy(this->children[i].brick,
                        this->children[i].neighbours[4]->brick,
                        2);
                this->children[i].AverageBrick();
                this->children[i].neighbours[4]->AverageBrick();
            }
        }
    }
}

const static int edgeoffsets[3][3] = {{6,1,9}, //
                                  {2,3,9}, // LEFT/RIGHT
                                  {18,1,3}}; // TOP/DOWN


void ChildrenEdgeCopy(Octree * t, int c1, int c2)
{
    if (t->children[c1].type == LEAF &&t->children[c2].type  == LEAF)
    {
        *t->children[c1].voxel = *t->children[c1].voxel + *t->children[c2].voxel;
        *t->children[c1].voxel = *t->children[c1].voxel / 2.f;
        *t->children[c2].voxel = *t->children[c1].voxel;
    }
}

void EdgeCopy(Voxel * brick1, Voxel * brick2, int orient)
{
    int index1, index2;
    for (int i = 0; i < 9; i++)
    {
        index1 = edgeoffsets[orient][0] + (i % 3) * edgeoffsets[orient][1]
            + floor(i/3) * edgeoffsets[orient][2];
        index2= (i % 3) * edgeoffsets[orient][1]
            + floor(i/3) * edgeoffsets[orient][2];

        brick1[index1] = brick1[index1] + brick2[index2];
        brick1[index1] = brick1[index1]/ 2;

        brick2[index2] = brick1[index1];
    }
}

void Octree::PrintBrick()
{
    for (int i = 0; i < 3; i++)
    {
        for(int j=0; j <3; j++)
        {
            for(int k=0; k<3; k++)
            {
                 cout << to_string(this->brick[i*9+j*3+k].col) << " ";
                 cout << (this->brick[i*9+j*3+k].occ) << " ";
            }
            cout << endl;
        }
    }
}
