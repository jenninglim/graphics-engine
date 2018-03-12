#ifndef RAY_H
#define RAY_H

#include "BVH.h"
#include "Intersection.h"
#include "Light.h"

using namespace glm;
// Ray
class Ray
{
    public:
        glm::vec4 initial;
        glm::vec4 direction;

    Ray()
    {
        initial = vec4(0);
        direction = vec4(0);
    }
    Ray(glm::vec4 start, glm::vec4 dir)
        :initial(start), direction(dir)
    {}

};

Ray reflect(const Ray I, const Intersection i);
Ray refract(const Ray I, const Intersection i);
void shootRay(const Ray r, Intersection &i, vec3 &colour, BVH bvh, Light light);
//void fresnel(const Ray r, const Intersection i, float &kr);

#endif
