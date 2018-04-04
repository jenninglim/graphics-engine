#ifndef BOUNDINGVOL_H
#define BOUNDINGVOL_H

#include <glm/glm.hpp>

using glm::vec3;

class BoundingVolume
{
    public:
        vec3 min;
        vec3 max;
        BoundingVolume();
        BoundingVolume(vec3 min, vec3 max);
        bool pointInside(vec3 point);
};
bool pointInsideAABB(vec3 point, vec3 min, vec3 max);

#endif
