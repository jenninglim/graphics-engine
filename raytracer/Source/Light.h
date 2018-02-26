#ifndef LIGHT_H
#define LIGHT_H

#include "Intersection.h"
#include <glm/glm.hpp>

using namespace std;
using glm::vec4;
using glm::vec3;

class Light
{
    public:
        vec4 position;
        vec3 power;
        vec3 indirect_light;

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

vec3 DirectLight(const Intersection& i,
        vector<Triangle> triangles,
        Light light);
#endif
