#ifndef LIGHT_H
#define LIGHT_H

using glm::vec4;
using glm::vec3;

class Light
{
    public:
        vec4 position;
        vec3 colour;

    Light(vec4 position, vec3 colour)
        :position(position), colour(colour)
    {

    }


};

#endif
