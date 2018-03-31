#ifndef LIGHT_H
#define LIGHT_H

#include <Config.h>


using namespace glm;
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
        this->indirect_light = (float)INDIRECT_LIGHT * vec3(1,1,1);
        this->position = vec4(0, -0.5, -0.7, 1);
        this->power = (float)LIGHT_POWER * vec3(1,1,1);
    }

    Light(vec4 position, vec3 power, vec3 indirect_light)
        :position(position), power(power), indirect_light(indirect_light)
    {
        
    }

    void forward(){
        this->position = this->position + vec4(0,0,1,0); 	
    }
    void backward(){
        this->position = this->position + vec4(0,0,-1,0);
    }
    void left(){
        this->position = this->position + vec4 (1,0,0,0);

    }
    void right(){
        this->position = this->position + vec4(-1,0,0,0);
    }


};

#endif
