#ifndef SHADER_H
#define SHADER_H

#include "Ray.h"
#include "Config.h"
#include "Light.h"
#include "BVH.h"
#include "Octree.h"

Ray reflect(const Ray I, const Intersection i);
Ray refract(const Ray I, const Intersection i);
void shootRay(const Ray r, vec3 &colour, BVH bvh, Light light);
float shootRay(const Ray r, vec3 &colour, Octree tree, Light l);

#endif
