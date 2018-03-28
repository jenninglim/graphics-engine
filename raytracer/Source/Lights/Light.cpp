#include "Light.h"
#include "glm/ext.hpp"
#include "Camera.h"
#include "Collision.h"
#include "Ray.h"

using namespace std;
using glm::vec3;
using glm::vec4;

vec3 DirectLight(const Intersection i, BVH bvh, Light light)
{
    vec3 norm = vec3(i.normal);
    vec3 r_hat = vec3(glm::normalize(light.position - i.position));

    float dist = glm::length(light.position - i.position);
    vec3 n_hat = glm::normalize(norm);

    vec3 lightColour = light.power * glm::max(glm::dot(r_hat, n_hat), 0.0f) /
        (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));

   lightColour += light.indirect_light;
    return lightColour;
}

vec3 ShadowLight(const Intersection i, BVH bvh, Light
        light)
{
    vec3 r_hat = vec3(glm::normalize(light.position - i.position));
    vec3 shadowMult = vec3(1);
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
                shadowMult =  vec3(0,0,0);
            }
    }
    return shadowMult;
}
