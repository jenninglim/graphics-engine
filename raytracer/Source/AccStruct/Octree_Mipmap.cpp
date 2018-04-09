#include "Octree.h"
#include "glm/ext.hpp"

using namespace glm;

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

