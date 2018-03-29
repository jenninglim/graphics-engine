#ifndef RAY_H
#define RAY_H

#include "Intersection.h"

using namespace glm;
// Ray
class Ray
{
    public:
        glm::vec4 initial;
        glm::vec3 direction;

    Ray()
    {
        initial = vec4(0);
        direction = vec3(0);
    }
    Ray(glm::vec4 start, glm::vec3 dir)
        :initial(start), direction(dir)
    {}

};

class Cone
{
    public:
        vec4 initial;
        vec3 direction;
        float theta;
        Cone()
    {
        initial = vec4(0);
        direction = vec3(0);
    }

        Cone(glm::vec4 start, glm::vec3 dir, float theta)
        :initial(start), direction(dir), theta(theta)
    {}


};
#endif
