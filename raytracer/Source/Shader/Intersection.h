#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>

struct Intersection
{
    glm::vec4 position;
    glm ::vec3 colour;
    float distance;
    glm::vec4 normal;
    float reflect_ratio;
    float refract_ratio;
    float ior;
    float area;
};


#endif
