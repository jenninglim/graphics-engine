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
        vec3 diffuseLightIntensity;
        vec3 ambientLightIntensity;
        vec3 specularLightIntensity;

    Light()
    {
        this->ambientLightIntensity = (float)AMBIENT_INTENSITY * vec3(1,1,1);
        this->position = vec4(0, -0.5, -0.7, 1);
        this->diffuseLightIntensity = (float)DIFFUSE_INTENSITY * vec3(1,1,1);
        this->specularLightIntensity = (float)SPECULAR_INTENSITY * vec3(1,1,1);
    }
    /*
    Light(vec4 position, vec3 specularLightIntensity, vec3 ambientLightIntensity, vec3 diffuseLightIntensity)
        :position(position), specularLightIntensity(specularLightIntensity),
        ambientLightIntensity(ambientLightIntensity), diffuseLightIntensity(diffuseLightIntensity)
    {

    }
    */
    void forward(){
        this->position = this->position + vec4(0,0,0.1,0);
    }
    void backward(){
        this->position = this->position + vec4(0,0,-0.1,0);
    }
    void left(){
        this->position = this->position + vec4 (-0.1,0,0,0);
    }
    void right(){
        this->position = this->position + vec4(0.1,0,0,0);
    }
    void down(){
        this->position = this->position + vec4(0,0.1,0,0);
    }
    void up(){
        this->position = this->position + vec4(0,-0.1,0,0);
    }
};

#endif
