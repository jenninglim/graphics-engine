#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <glm/glm.hpp>
#include "Ray.h"

using namespace glm;

class Sphere : public Object
{
    public:
        float radius;
        vec4 centre;
    Sphere(const vec4 c, const float r);
    bool intersection(
            const Ray r,
            Intersection &closestI);
    bool intersection(
            const Cone r,
            Intersection &closestI);
    bool boxOverlap(vec3 boxcenter, vec3 boxhalfisze);

};

#endif
