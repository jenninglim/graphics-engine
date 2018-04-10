#include "Octree.h"
#include <csignal>
#include "glm/ext.hpp"

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

void EdgeCopy(Cell * brick1, Cell * brick2, int orient);
void ChildrenEdgeCopy(Octree * t, int c1, int c2);

void Octree::updateTexture()
{
    int index = 0;
    int acc=0;
    int other=0;

    if (this->type == NODE)
    {
        //BrickEdgeCopy();
                for (int i = 0; i < 3 * 3 *3; i++)
        {
            this->brick[i] = Cell();
        }
        for (int i = 0; i < 8; i ++)
        {
            index = (i % 4) + cubeOffset[i % 4] + 9*floor(i / 4);
            if (this->children[i].type != EMPTY)
            {
                this->brick[index] = *children[i].voxel;
            }
            else
            {
                acc = 0;
                other=0;
                //compute Identity
                for (int j = 0; j < 3; j++)
                {
                    other = i+idenoffset[i][j];
                    if (this->children[other].type != EMPTY)
                    {
                        
                        this->brick[index] = this->brick[index]+*this->children[other].voxel;
                        acc++;
                    }
                }
                if(acc>0)
                {
                this->brick[index] = this->brick[index] / (float) acc;
                }
            }
        }
        this->mipmap();

        Cell cell = Cell();
        int count =0;;

        for (int i = 0; i < 8; i++)
        {
            if (this->children[i].type == NODE)
            {
                cell = cell +
                    (this->children[i].brick[weightoffset[i][0]] * 0.4f
                     + this->children[i].brick[weightoffset[i][1]] * 0.2f
                     + this->children[i].brick[weightoffset[i][2]] * 0.2f
                     + this->children[i].brick[weightoffset[i][3]] * 0.2f);
                count++;
            }
            else if (this->children[i].type == LEAF)
            {
                cell = cell + * this->children[i].voxel;
                count++;
            }
            
            /*
            if (this->children[i].type!= EMPTY)
            {
                cell = cell + *this->children[i].voxel;
                count++;
            }
            */

        }
        if (count >0)
        {
        *this->voxel = cell / (float) count;
        }
    }
}

void Octree::makeTexture(const vec3 colour)
{
    this->voxel = new Cell();
    if (this->type == LEAF)
    {
        this->voxel->col = colour;
    }
    else if (this->type == NODE)
    {
        this->brick = new Cell[3 * 3 * 3];
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
if(isnan(brick[index].occ))
            {
                asm("int $3");
            }
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i] + mipmapoffset[3][i]; // edge assignment
if(isnan(brick[index].occ))
            {
                asm("int $3");
            }
                this->brick[index] = this->brick[index]+this->brick[index + mipmapoffset[3][i]];
                this->brick[index] = this->brick[index]/ 2;
                if(isnan(brick[index].occ))
            {
                asm("int $3");
            }


            }
        }
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

void Octree::BrickEdgeCopy()
{    
    LeafEdgeCopy(); 
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i + (int)floor(i/2) *2].type == NODE &&
                this->children[i+ (int)floor(i/2) *2+2].type == NODE)
        {
            EdgeCopy(this->children[i+ (int)floor(i/2) * 2].brick,
                    this->children[i+2+ (int)floor(i/2) *2].brick,
                    0);
        }

        if (this->children[2*i].type == NODE &&
                this->children[2*i+1].type == NODE)
        {
            EdgeCopy(this->children[2*i].brick,
                    this->children[2*i+1].brick,
                    1);
        }

        if (this->children[i].type == NODE &&
                this->children[i+4].type == NODE)
        {
            EdgeCopy(this->children[i].brick,
                    this->children[i+4].brick,
                    2);
        }
    }
}

const static int edgeoffsets[3][3] = {{6,1,9}, //
                                  {2,3,9}, // LEFT/RIGHT
                                  {9,1,3}}; // TOP/DOWN


void Octree::LeafEdgeCopy()
{
    int index1, index2;
    for (int i = 0; i < 4; i ++)
    {
        ChildrenEdgeCopy(this, i,i+2);
        ChildrenEdgeCopy(this, 2*i,2*i+1);
        ChildrenEdgeCopy(this, i,i + 4);
    }
}

void ChildrenEdgeCopy(Octree * t, int c1, int c2)
{
    if (t->children[c1].type == LEAF &&t->children[c2].type  == LEAF)
    {
        *t->children[c1].voxel = *t->children[c1].voxel + *t->children[c2].voxel;
        *t->children[c1].voxel = *t->children[c1].voxel / 2.f;
        *t->children[c2].voxel = *t->children[c1].voxel;
    }
}

void EdgeCopy(Cell * brick1, Cell * brick2, int orient)
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
