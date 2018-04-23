#ifndef CELL_H
#define CELL_H

#include "glm/ext.hpp"
#include "Intersection.h"
#include <stdio.h>
#include <iostream>

using namespace glm;

struct Cell{
    vec3 col;
    float occ;
    float irrad;

    Cell operator+(const Cell &c) const;
    Cell operator*(const float x) const;
    Cell operator/(const float x);
    Cell();
    virtual vec3 getNorm()
    {
        std::cout << "get norm virtual called" << std::endl;
        assert(1 < 0);
        return vec3(0);
    }
};

struct CellLeaf : public Cell
{
    vec3 norm;

    Cell operator+(const Cell &c);
    CellLeaf();
    CellLeaf(const Intersection inter);
    vec3 getNorm();
};


#endif
