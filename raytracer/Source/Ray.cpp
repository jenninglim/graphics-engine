#include "Ray.h"
Ray reflect(const Ray I, const Intersection i) 
{ 
    vec4 dir = I.direction - 2 * glm::dot(I.direction, i.normal) * i.normal;
    return  Ray(i.position, dir);
} 

Ray refract(const Ray r, const Intersection i)
{
    float cosi = glm::clamp(glm::dot(r.direction, i.normal),- 1.f, 1.f );
    float etai = 1.f, etat = i.refract;
    vec4 n = i.normal;
    if (cosi < 0) {cosi = - cosi;}
    else { std::swap(etai, etat);n = -n; }
    float eta = etai/etat;
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    vec4 dir =  k < 0.f ? vec4(0) : eta * r.direction + (eta * cosi - glm::sqrt(k)) * n;  
    return Ray(i.position, dir);
}
