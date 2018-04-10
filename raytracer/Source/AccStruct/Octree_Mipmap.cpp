#include "Octree.h"
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
                                {8,7,5,11},
                                {6,7,3,15},
                                {2,1,5,11},
                                {0,1,3,9}};

void EdgeCopy(Cell * brick1, Cell * brick2, int orient);

void Octree::updateTexture()
{
    if (this->type == NODE)
    {
        BrickEdgeCopy();
        this->brick = new Cell[3 * 3 * 3];
        for (int i = 0; i < 3 * 3 *3; i++)
        {
            this->brick[i] = Cell();
        }
        int index = 0;

        for (int i = 0; i < 8; i ++)
        {
            index = (i % 4) + cubeOffset[i % 4] + 9*floor(i / 4);
            if (this->children[i].type != EMPTY)
            {
                this->brick[index].col = children[i].voxel->col;
                this->brick[index].occ = children[i].voxel->occ;
            }
        }
        this->mipmap();

        Cell cell = Cell();

        for (int i = 0; i < 8; i++)
        {
            if (this->children[i].type == NODE)
            {
                cell = cell +
                    (this->children[i].brick[weightoffset[i][0]] * 0.4f
                     + this->children[i].brick[weightoffset[i][1]] * 0.2f
                     + this->children[i].brick[weightoffset[i][2]] * 0.2f
                     + this->children[i].brick[weightoffset[i][3]] * 0.2f) * 1/8.f;
            }
            else if (this->children[i].type == LEAF)
            {
                cell.col += 1.f/8.f * this->voxel->col;
                cell.occ += 1.f/8.f * this->voxel->occ;
            }
        }

        *this->voxel = cell;
        //this->voxel->occ = this->voxel->brick[13].occ;
    }
}

void Octree::makeTexture(const vec3 colour)
{
    this->voxel = new Cell();
    this->voxel->occ = 0;
    if (this->type == LEAF)
    {
        this->voxel->col = colour;
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
    
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i].type == NODE &&
                this->children[i+2].type == NODE)
        {
            EdgeCopy(this->children[i].brick,
                    this->children[i+2].brick,
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



void EdgeCopyLeaf(Octree * t1, Octree * t2, int orient)
{
    int index1, index2;
    for (int i = 0; i < 4; i ++)
    {

    }
}

void EdgeCopy(Cell * vox1, Cell * vox2)

{
    
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
