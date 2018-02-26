#ifndef LIGHT_H
#define LIGHT_H

using glm::vec4;
using glm::vec3;

class Light
{
    public:
        vec4 position;
        vec3 power;
        vec3 indirect_light;
    Light(vec4 position, vec3 power )
        :position(position), power(power)
    {
        this->indirect_light = vec3(1,1,1) * 0.0f;
    }
};

#endif
