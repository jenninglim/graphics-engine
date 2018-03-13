#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <glm/glm.hpp>
using namespace glm;

class Sphere : public Object
{
    public:
        float radius;
        vec4 centre;
    Sphere(const vec4 c, const float r);
};

#endif
