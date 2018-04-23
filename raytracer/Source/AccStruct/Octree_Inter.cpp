#include "Octree.h"
#include "MyMath.h"

static const int corners[8] = {0,3,6,8,18,21,24,26};
float Octree::interOcc(vec3 point)
{

    float data[8];
    for (int i = 0; i < 8; i++)
    {
        data[i] = 0;
        data[i] = this->brick[corners[i]].occ;
    }
    return trilinear<float>(point - this->centre + this->boxHalfSize,
            data[7], //  c000
            data[6], //  c100
            data[3], //  c010
            data[2], //  c110
            data[5], //  c001
            data[4], //  c101
            data[1], //  c011
            data[0]  //  c111
            ); 
}

vec3 Octree::interCol(vec3 point)
{
    vec3 data[8]; 

    for (int i = 0; i < 8; i++)
    {
        data[i] = vec3(0);
        if (this->children[i].voxel != NULL && this->children[i].type != EMPTY)
        {
            data[i] = this->children[i].voxel->col;
        }
    }
    return trilinear<vec3>(point - this->centre + this->boxHalfSize,
            data[7], //  c000
            data[6], //  c100
            data[3], //  c010
            data[2], //  c110
            data[5], //  c001
            data[4], //  c101
            data[1], //  c011
            data[0]  //  c111
            ); 
}
