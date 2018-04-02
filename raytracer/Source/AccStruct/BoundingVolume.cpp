#include "BoundingVolume.h"

using glm::vec3;

BoundingVolume::BoundingVolume()
{
    this->max = vec3(20);
    this->min = vec3(-20);
}

BoundingVolume::BoundingVolume(vec3 min, vec3 max)
{
    this->min = min;
    this->max = max;
}
