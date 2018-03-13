#ifndef SHADER_H
#define SHADER_H

#include "Ray.h"
#include "Config.h"
#include "BVH.h"

Ray reflect(const Ray I, const Intersection i);
Ray refract(const Ray I, const Intersection i);
void shootRay(const Ray r, vec3 &colour, BVH bvh, Light light);

#endif
