#include "Sphere.h"

#ifdef DEBUG
#include <iostream>
#include "glm/ext.hpp"
#endif

Sphere :: Sphere(const vec4 c, const float r)
{
    this->centre = c;
    this->radius = r;
    this->reflect_ratio = 1.0f;
    this->refract_ratio = 1.f;
    this->ior = DEF_IOR;
    this->colour = vec3(1.0f,1.0f,1.0f);
}

bool solveQuadratic(const float &a,
        const float &b,
        const float &c,
        float &x0,
        float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) 
    {
        return false;
    }
    else if (discr == 0)
    {
        x0 = x1 = - 0.5 * b / a;
    }
    else
    {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

bool Sphere::intersection(const Ray r, Intersection &closestI)
{
    bool intersectionFound = true;
    vec3 cent = vec3(this->centre[0],
            this->centre[1],
            this->centre[2]);
    vec3 dir = vec3(r.direction[0],
            r.direction[1],
            r.direction[2]);
    vec3 init = vec3(r.initial[0],
            r.initial[1],
            r.initial[2]);
    float t;

    // analytic solution
    vec3 L = init - cent;
    float b = glm::dot(L, dir); 
    float c = glm::dot(L,L) - this->radius * this->radius; 
    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f)
    {
        intersectionFound = false;
    }
    float discr = b*b - c;
    // A negative discriminant corresponds to ray missing sphere
    if (discr < 0.0f)
    {
        intersectionFound = false;
    }
    // Ray now found to intersect sphere, compute smallest t value of intersection
    t = -b - glm::sqrt(discr);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.0f)
    {
        t = 0.0f;
    }
    if (intersectionFound)
    {
        closestI.position = t * vec4(r.direction,0) + r.initial;
        closestI.distance = t;
        closestI.colour = this->colour;
        closestI.normal = glm::normalize(r.initial - this->centre);
        closestI.reflect_ratio = this->reflect_ratio;
        closestI.refract_ratio = this->refract_ratio;
        closestI.ior = this->ior;
    }

    return intersectionFound;
}
