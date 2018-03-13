#include "Light.h"
#include "glm/ext.hpp"
#include "Camera.h"
#include "Ray.h"

using namespace std;
using glm::vec3;
using glm::vec4;

vec3 DirectLight(const Intersection& i, BVH bvh, Light light)
{
    vec3 norm = vec3(i.normal[0],
            i.normal[1],
            i.normal[2]);
    vec4 r_hat4 = glm::normalize(light.position - i.position);
    vec3 r_hat = vec3(r_hat4[0],
            r_hat4[1],
            r_hat4[2]);
    float dist = glm::length(light.position - i.position);
    vec3 n_hat = glm::normalize(norm);

    vec3 lightColour = light.power * glm::max(glm::dot(r_hat, n_hat), 0.0f) /
        (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));

    Intersection closestIntersection = {
                light.position,
                vec3(0),
                std::numeric_limits<float>::max(),
                vec4(0)};

    if (collision(bvh, Ray(i.position, r_hat), closestIntersection))
    {
         if (closestIntersection.distance < glm::length(light.position - i.position) &&
                 closestIntersection.distance > EPSILON)
            {
                lightColour =  vec3(0,0,0);
            }
    }
    lightColour += light.indirect_light;
    return lightColour;
}
