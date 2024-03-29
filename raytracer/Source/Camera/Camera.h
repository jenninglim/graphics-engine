#ifndef  CAMERA_H
#define CAMERA_H

#include "Config.h"
using namespace glm;

//Camera
class Camera
{
    public:
        float focalLength;
        glm::vec4 position;
        glm::mat4 R;
        float yaw; // rotation of the camera
        float distance;
        mat4 increR;

    Camera()
    {
        this->position =  vec4(0,0,-3,1);
        this->focalLength = CAM_FOCAL_LENGTH;
        yaw = 0;
        R = mat4(1.0);
        vec4 v0(glm::cos(ROTATION_SPEED), 0, -glm::sin(ROTATION_SPEED),0);
        vec4 v1(glm::sin(ROTATION_SPEED), 0, glm::cos(ROTATION_SPEED),0);
        vec4 v2(0, 1, 0,0);
        this->increR = mat4(v0,v2,v1,vec4(0,0,0,1));
    }
    Camera(float focalLength, glm::vec4 cameraPos)
        :focalLength(focalLength), position(cameraPos)
    {
        yaw = 0;
        R = mat4(1.0);
        vec4 v0(glm::cos(ROTATION_SPEED), 0, -glm::sin(ROTATION_SPEED),0);
        vec4 v1(glm::sin(ROTATION_SPEED), 0, glm::cos(ROTATION_SPEED),0);
        vec4 v2(0, 1, 0,0);
        this->increR = mat4(v0,v2,v1,vec4(0,0,0,1));
    }

    void forward(){
        this->position = this->position + glm::vec4(0,0,1,0);
    }
    void backward(){
        this->position = this->position + glm::vec4(0,0,-1,0);
    }
    void left(){
        this->yaw = yaw + ROTATION_SPEED;
        updateRotation();
        this->position = this->increR * this->position;

    }
    void right(){
        this->yaw = yaw - ROTATION_SPEED;
        updateRotation();
        this->position = glm::inverse(this->increR) * this->position;
    }

    private:
    void updateRotation()
    {
        vec4 v0(glm::cos(yaw), 0, -glm::sin(yaw),0);
        vec4 v1(glm::sin(yaw), 0, glm::cos(yaw),0);
        vec4 v2(0, 1, 0,0);
        this->R = mat4(v0,v2,v1,vec4(0,0,0,1));
    }

};
#endif
