#include "Octree.h"
#include "MyMath.h"

static const int corners[8] = {0,2,6,8,18,20,24,26};
float Octree::interOcc(vec3 point)
{

    float data[8];
    if (this->type != EMPTY)
    {
        for (int i = 0; i < 8; i++)
        {
            data[i] = this->brick[corners[i]].occ;
        }
    }
    else
    {
        int index;
        for (int i = 0; i < 8; i++)
        {
            index = 4 + floor(i /4);
            data[i] = 0;

            if (this->neighbours[index] != NULL)
            {
                if (this->neighbours[index]->type != EMPTY)
                {
                    data[i] = this->neighbours[index]->brick[corners[(int) (4 * (1-2*floor(i/4))) + i]].occ;
                }
                else
                {
                }
            }
        }
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
    if (this->type != EMPTY)
    {
        for (int i = 0; i < 8; i++)
        {
            data[i] = this->brick[corners[i]].col;
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

Cell Octree::interVox(vec3 point)
{
    Cell data[8]; 
    if (this->type != EMPTY)
    {
        for (int i = 0; i < 8; i++)
        {
            data[i] = this->brick[corners[i]];
        }
    }
    
    return trilinear<Cell>(point - this->centre + this->boxHalfSize,
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
