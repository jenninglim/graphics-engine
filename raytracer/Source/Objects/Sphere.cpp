#include "Sphere.h"

Sphere :: Sphere(const vec4 c, const float r)
{
    this->centre = c;
    this->radius = r;
}

bool Sphere::intersection(const Ray r, Intersection &closestI)
{
    return false;
}
