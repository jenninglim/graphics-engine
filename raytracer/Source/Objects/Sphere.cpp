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
    float t, t0, t1;

    // analytic solution
    vec3 L = cent-init;
    float tca = glm::dot(L, dir); 
    float d2 = glm::dot(L,L) - tca*tca;
    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (d2 > this->radius*this->radius || tca < 0.0f)
    {
        intersectionFound = false;
    }
    float thc = glm::sqrt(this->radius*this->radius - d2);
    // A negative discriminant corresponds to ray missing sphere
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) std::swap(t0, t1); 

    if (t0 < 0) { 
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0) intersectionFound = false; // both t0 and t1 are negative 
    } 
    t = t0; 

    if (t < closestI.distance)
    {
        closestI.position = t * vec4(r.direction,0) + r.initial;
        closestI.distance = t;
        closestI.colour = this->colour;
        closestI.normal = glm::normalize(closestI.position - this->centre);
        closestI.reflect_ratio = this->reflect_ratio;
        closestI.refract_ratio = this->refract_ratio;
        closestI.ior = this->ior;
    }

    return intersectionFound;
}
