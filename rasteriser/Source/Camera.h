#ifndef  CAMERA_H
#define CAMERA_H
#define ROTATION_SPEED 0.1 // In radians

using namespace glm;

// Ray
class Ray
{
    public:
        glm::vec4 initial;
        glm::vec4 direction;

    Ray(glm::vec4 start, glm::vec4 dir)
        :initial(start), direction(dir)
    {}

};

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

    Camera(float focalLength, glm::vec4 cameraPos)
        :focalLength(focalLength), cameraPos(cameraPos)
    {
        yaw = 0;
        R = mat4(1.0);
        vec4 v0(glm::cos(ROTATION_SPEED), 0, -glm::sin(ROTATION_SPEED),0);
        vec4 v1(glm::sin(ROTATION_SPEED), 0, glm::cos(ROTATION_SPEED),0);
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
