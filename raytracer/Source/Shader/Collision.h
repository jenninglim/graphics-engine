#ifndef COLLISION_H
#define COLLISION_H

#include "BVH.h"
#include "Ray.h"
#include <glm/glm.hpp>

bool collision(BVH bvh, Cone r, Intersection &closestI);

#endif
