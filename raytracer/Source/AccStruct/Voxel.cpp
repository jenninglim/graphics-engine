#include "Voxel.h"
Voxel::Voxel()
{
    this->col = vec3(0);
    this->occ = 0;
    this->irrad = 0;
}
Voxel Voxel::operator+ (const Voxel &c)
{
    Voxel vox;
    vox.col = this->col + c.col;
    vox.occ = this->occ + c.occ;
    vox.irrad = this->irrad + c.irrad;
    return vox;
}

Voxel Voxel::operator* (const float x)
{
    Voxel cell;
    cell.col = this->col * x;
    cell.occ = this->occ * x;
    cell.irrad = this->irrad * x;
    return cell;
}

Voxel Voxel::operator/ (const float x)
{
    Voxel cell;
    cell.col = this->col / x;
    cell.occ = this->occ / x;
    cell.irrad = this->irrad / x;
    return cell;
}

VoxelLeaf::VoxelLeaf()
{
    this->col = vec3(0);
    this->occ = 0.f;
    this->norm = vec3(1,0,0);
    this->irrad = 0.f;
}

Voxel VoxelLeaf::operator+(const Voxel &c)
{
    Voxel vox;
    vox.col = this->col + c.col;
    vox.occ = this->occ + c.occ;
    vox.irrad = this->irrad + c.irrad;
    return vox;
}

VoxelLeaf::VoxelLeaf(const Intersection inter)
{
    this->col = inter.colour; 
    this->occ = 0.f;
    this->norm = vec3(inter.normal);
    this->irrad = 0;
}

vec3 VoxelLeaf::getNorm()
{
    return this->norm;
}
