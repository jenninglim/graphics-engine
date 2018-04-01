#ifndef  CAMERA_H
#define CAMERA_H

#include "Config.h"
#include "glm/gtx/string_cast.hpp"
using namespace glm;

//Camera
class Camera
{

    public:
        float focalLength;
        glm::vec4 cameraPos;
        glm::mat4 R;
        float yaw; // rotation of the camera
        float distance;
        mat4 increR;
        float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    Camera()
    {
        this->cameraPos =  vec4(0,0,-4,1);
        this->focalLength = CAM_FOCAL_LENGTH;
        yaw = 0;
        R = mat4(1.0);
        vec4 v0(glm::cos(ROTATION_SPEED), 0, glm::sin(ROTATION_SPEED),0);
        vec4 v1(-1*glm::sin(ROTATION_SPEED), 0, glm::cos(ROTATION_SPEED),0);
        vec4 v2(0, 1, 0,0);
        this->increR = mat4(v0,v2,v1,vec4(0,0,0,1));
    }

    Camera(float focalLength, glm::vec4 cameraPos)
        :focalLength(focalLength), cameraPos(cameraPos)
    {
        yaw = 0;
        R = mat4(1.0);
        vec4 v0(glm::cos(ROTATION_SPEED), 0, glm::sin(ROTATION_SPEED),0);
        vec4 v1(-glm::sin(ROTATION_SPEED), 0, glm::cos(ROTATION_SPEED),0);
        vec4 v2(0, 1, 0,0);
        this->increR = mat4(v0,v2,v1,vec4(0,0,0,1));
    }

    void forward(){
        this->cameraPos = this->cameraPos + glm::vec4(0,0,1,0);
    }
    void backward(){
        this->cameraPos = this->cameraPos + glm::vec4(0,0,-1,0);

    }
    void left(){
        this->yaw = yaw + ROTATION_SPEED;
        updateRotation();
        this->cameraPos = this->increR * this->cameraPos;


    }
    void right(){
        this->yaw = yaw - ROTATION_SPEED;
        updateRotation();
        this->cameraPos = glm::inverse(this->increR) * this->cameraPos;

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
