#ifndef LIGHT_H
#define LIGHT_H

using glm::vec4;
using glm::vec3;

class Light
{
    public:
        vec4 position;
        vec3 colour;
        vec3 indirect_light;

    Light(vec4 position, vec3 colour)
        :position(position), colour(colour)
    {
        this->indirect_light = vec3(1,1,1) * 0.5f;
    }

    Light(vec4 position, vec3 colour, vec3 indirect_light)
        :position(position), colour(colour), indirect_light(indirect_light)
    {
       // WHAT DO?? 
    }
    


};

#endif
