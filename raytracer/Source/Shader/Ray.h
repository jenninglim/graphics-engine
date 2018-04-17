#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

using namespace glm;
// Ray
class Ray
{
    public:
        glm::vec3 initial;
        glm::vec3 direction;

    Ray()
    {
        initial = vec3(0);
        direction = vec3(0);
    }
    Ray(glm::vec3 start, glm::vec3 dir)
        :initial(start), direction(dir)
    {}
    
    Ray reflect(vec3 norm, vec3 point);
};

class Cone
{
    public:
        vec3 initial;
        vec3 direction;
        float theta;
        Cone()
    {
        initial = vec3(0);
        direction = vec3(0);
    }

        Cone(glm::vec3 start, glm::vec3 dir, float theta)
        :initial(start), direction(dir), theta(theta)
    {}

    Cone reflect(vec3 norm, vec3 point);
};
#endif
