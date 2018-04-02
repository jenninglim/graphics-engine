#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "BVH.h"
#include "Intersection.h"

using namespace std;
using glm::vec4;
using glm::vec3;

class Light
{
    public:
        vec4 position;
        vec3 power;
        vec3 indirect_light;

    Light()
    {
        this->indirect_light = vec3(1,1,1) * 0.5f;
        this->position = vec4(0, -0.5, -0.7, 1);
        this->indirect_light = 14.f * vec3(1,1,1);
    }

    Light(vec4 position, vec3 pow)
        :position(position), power(pow)
    {
        this->indirect_light = vec3(1,1,1) * 0.5f;
    }

    Light(vec4 position, vec3 power, vec3 indirect_light)
        :position(position), power(power), indirect_light(indirect_light)
    {
       // WHAT DO?? 
    }
};

vec3 DirectLight(const Intersection i,
        BVH bvh,
        Light light);

float ShadowLight(const Intersection i,
        BVH bvh,
        Light light);

#endif
