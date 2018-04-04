#include "BoundingVolume.h"

using glm::vec3;

BoundingVolume::BoundingVolume()
{
    this->max = vec3(2);
    this->min = vec3(-2);
}

BoundingVolume::BoundingVolume(vec3 min, vec3 max)
{
    this->min = min;
    this->max = max;
}

bool BoundingVolume::pointInside(vec3 point)
{
    for (int i = 0; i < 3; i++)
    {
        if (point[i] > max[i] || point[i] < min[i])
        {
            return false;
        }
    }
    return true;
}
