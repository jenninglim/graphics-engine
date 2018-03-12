#include "Ray.h"
Ray reflect(const Ray I, const Intersection i) 
{ 
    vec4 dir = I.direction - 2 * glm::dot(I.direction, i.normal) * i.normal;
    return  Ray(i.position, dir);
} 
