#ifndef CELL_H
#define CELL_H

#include "glm/ext.hpp"
using namespace glm;

struct Cell{
    vec3 col;
    float occ;

    Cell operator+(const Cell &c);
    Cell operator*(const float x);
    Cell operator/(const float x);
    Cell();
};

#endif
