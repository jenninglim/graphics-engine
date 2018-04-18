#include "Ray.h"

Ray Ray::reflect(vec3 norm, vec3 point)
{
    vec3 dir = this->direction - 2 * glm::dot(this->direction, norm) * norm;
    dir = glm::normalize(dir);
    return Ray(point, dir);
}

Cone Cone::reflect(vec3 norm, vec3 point)
{
    vec3 dir = this->direction - 2 * glm::dot(this->direction, norm) * norm;
    dir = glm::normalize(dir);
    return Cone(point, dir, this->theta);
}
