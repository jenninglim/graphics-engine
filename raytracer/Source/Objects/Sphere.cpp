#include "Sphere.h"
#include "Config.h"

#ifdef DEBUG
#include <iostream>
#include "glm/ext.hpp"
#endif

Sphere :: Sphere(const vec4 c, const float r)
{
    this->centre = c;
    this->radius = r;
    this->reflect_ratio = SPHERE_REFLECTANCE;
    this->refract_ratio = SPHERE_REFRACT;
    this->ior = DEF_IOR;
    this->colour = vec3(0.5,1.0f,1.0f);
}

bool Sphere::intersection(const Ray r, Intersection &closestI)
{
    bool intersectionFound = true;
    vec3 cent = vec3(this->centre);
    vec3 dir = vec3(r.direction);
    vec3 init = vec3(r.initial);
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

    if (t0 < -EPSILON) { 
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < -EPSILON) return false; // both t0 and t1 are negative 
    } 
    t = t0; 

    if (t > EPSILON && t < closestI.distance)
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
