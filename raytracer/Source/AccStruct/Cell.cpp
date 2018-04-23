#include "Cell.h"

Cell::Cell()
{
    this->col = vec3(0);
    this->occ = 0;
    this->irrad = 0;
}
Cell Cell::operator+ (const Cell &c) const
{
    Cell cell;
    cell.col = this->col + c.col;
    cell.occ = this->occ + c.occ;
    cell.irrad = this->irrad + c.irrad;
    return cell;
}

Cell Cell::operator* (const float x) const
{
    Cell cell;
    cell.col = this->col * x;
    cell.occ = this->occ * x;
    cell.irrad = this->irrad * x;
    return cell;
}

Cell Cell::operator/ (const float x)
{
    Cell cell;
    cell.col = this->col / x;
    cell.occ = this->occ / x;
    cell.irrad = this->irrad / x;
    return cell;
}

CellLeaf::CellLeaf()
{
    this->col = vec3(0);
    this->occ = 0.f;
    this->norm = vec3(1,0,0);
    this->irrad = 0.f;
}

Cell CellLeaf::operator+(const Cell &c)
{
    Cell vox;
    vox.col = this->col + c.col;
    vox.occ = this->occ + c.occ;
    vox.irrad = this->irrad + c.irrad;
    return vox;
}

CellLeaf::CellLeaf(const Intersection inter)
{
    this->col = inter.colour;
    this->occ = 0.f;
    this->norm = vec3(inter.normal);
    this->irrad = 0;
}

vec3 CellLeaf::getNorm()
{
    return this->norm;
}
