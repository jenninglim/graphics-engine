#include <limits>
#include <glm/glm.hpp>
#include "Object.h"
#include "Config.h"

#ifdef DEBUG
    #include <iostream>
    #include "glm/ext.hpp"
#endif

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


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
