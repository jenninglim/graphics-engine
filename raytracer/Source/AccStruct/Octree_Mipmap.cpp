#include "Octree.h"
#include "glm/ext.hpp"

using namespace glm;

static const int mipmapoffset[5][3] = {{0,2,4},
                                {6,6,6},
                                {18,18,18},
                                {1,-3,-9},
                                {9,9,3}}; // 

static const int cubeOffset[4] = {1,1,2,2};

void EdgeCopy(vec3 * brick1, vec3 * brick2, int orient);

void Octree::updateTexture()
{
    if (this->type == NODE)
    {
        BrickEdgeCopy();
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

const static int weightoffset[8][4] = {{26,25,23,17},
                                {24,25,21,15},
                                {20,19,23,11},
                                {18,19,21,9},
                                {8,7,5,11},
                                {6,7,3,15},
                                {2,1,5,11},
                                {0,1,3,9}};
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
                this->voxel->brick[index] = this->voxel->brick[index + mipmapoffset[3][i]];
                index = k + mipmapoffset[i][k] + j * mipmapoffset[4][i] + mipmapoffset[3][i]; // edge assignment
                this->voxel->brick[index] += this->voxel->brick[index + mipmapoffset[3][i]];
                this->voxel->brick[index] /= 2;
            }
        }
    }

    this->voxel->colour = vec3(0);
    for (int i = 0; i < 8; i++)
    {
        if (this->children[i].type == NODE)
        {
            this->voxel->colour += 1/8.f
                *(this->children[i].voxel->brick[weightoffset[i][0]] * 0.4f
                        + this->children[i].voxel->brick[weightoffset[i][1]] * 0.2f
                        + this->children[i].voxel->brick[weightoffset[i][2]] * 0.2f
                        + this->children[i].voxel->brick[weightoffset[i][3]] * 0.2f);
        }
        else if (this->children[i].type == LEAF)
        {
            this->voxel->colour += 1.f/8.f * this->voxel->colour;
        }
    }
    //this->voxel->colour = this->voxel->brick[14];
}

void Octree::PrintBrick()
{
    for (int i = 0; i < 3; i++)
    {
        for(int j=0; j <3; j++)
        {
            for(int k=0; k<3; k++)
            {
                cout << to_string(this->voxel->brick[i*9+j*3+k]) << " ";
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
            EdgeCopy(this->children[i].voxel->brick,
                    this->children[i+2].voxel->brick,
                    0);
        }

        if (this->children[2*i].type == NODE &&
                this->children[2*i+1].type == NODE)
        {
            EdgeCopy(this->children[2*i].voxel->brick,
                    this->children[2*i+1].voxel->brick,
                    1);
        }

        if (this->children[i].type == NODE &&
                this->children[i+4].type == NODE)
        {
            EdgeCopy(this->children[i].voxel->brick,
                    this->children[i+4].voxel->brick,
                    2);
        }
    }
    }

const static int edgeoffsets[3][3] = {{6,1,9}, //
                                  {2,3,9}, // LEFT/RIGHT
                                  {9,1,3}}; // TOP/DOWN

void EdgeCopy(vec3 * brick1, vec3 * brick2, int orient)
{
    int index1, index2;
    for (int i = 0; i < 9; i++)
    {
        index1 = edgeoffsets[orient][0] + (i % 3) * edgeoffsets[orient][1]
            + floor(i/3) * edgeoffsets[orient][2];
        index2= (i % 3) * edgeoffsets[orient][1]
            + floor(i/3) * edgeoffsets[orient][2];

        brick1[index1] += brick2[index2];
        brick1[index1] /= 2;

        brick2[index2] = brick1[index1];
    }

}
