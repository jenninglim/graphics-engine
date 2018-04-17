#ifndef VOXEL_H
#define VOXEL_H

#include "glm/ext.hpp"
#include "Intersection.h"

using namespace glm;

struct Voxel{
    vec3 col;
    float occ;
    float irrad;

    virtual Voxel operator+(const Voxel &c);
    Voxel operator*(const float x);
    Voxel operator/(const float x);
    Voxel();
    virtual vec3 getNorm()
    {
        return vec3(0);
    }
};

struct VoxelLeaf : public Voxel
{
    vec3 norm;

    Voxel operator+(const Voxel &c);
    VoxelLeaf();
    VoxelLeaf(const Intersection inter);
    vec3 getNorm();
};

#endif
