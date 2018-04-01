#include "Light.h"
#include "Camera.h"
#include "Collision.h"
#include "Ray.h"

#ifdef DEBUG
#include <iostream>
#include "glm/ext.hpp"
#endif

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

float ShadowLight(const Intersection i, BVH bvh, Light
        light)
{
    vec3 r_hat = glm::normalize(vec3(light.position - i.position));
    Intersection closestIntersection = {
                light.position,
                vec3(0),
                std::numeric_limits<float>::max(),
                vec4(0)};
    float cone_size = glm::atan(0.1f/glm::l2Norm(vec3(light.position - i.position)));
    if (collision(bvh, Cone(i.position, r_hat, cone_size), closestIntersection))
    {
        float dist = glm::l2Norm(vec3(light.position- i.position));
        if (closestIntersection.distance > dist)
        {
            return 1.f;
        }
        return glm::pow(1.f - closestIntersection.area,2);
    }

    return 1.f;
}
