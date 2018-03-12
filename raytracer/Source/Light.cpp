#include "Light.h"
#include "glm/ext.hpp"
#include "BVH.h"
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::vec4;

vec3 DirectLight(const Intersection& i, BVH bvh, Light light)
{
    vec4 r_hat = glm::normalize(light.position - i.position);
    float dist = glm::length(light.position - i.position);
    vec4 n_hat = glm::normalize(i.normal);

    vec3 lightColour = light.power * glm::max(glm::dot(r_hat, n_hat), 0.0f) /
        (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));

    Intersection closestIntersection = {
                light.position,
                vec3(0),
                std::numeric_limits<float>::max(),
                vec4(0)};

    if (collision(bvh, i.position, r_hat, closestIntersection))
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
