#include "Shader.h"
#include <stack>

#ifdef DEBUG
#include "glm/ext.hpp"
#include <iostream>
#endif

using namespace std;

typedef struct Ray_t
{
    Ray r;    
    int depth;
    float prevReflectance;
    Ray_t():r(),depth(0), prevReflectance(1.0f)
    {
    }
    Ray_t(Ray r, int d, double acc):
        r(r), depth(d), prevReflectance(acc)
    {}
} Ray_t;

Ray reflect(const Ray I, const Intersection i) 
{ 
    vec4 dir = I.direction - 2 * glm::dot(I.direction, i.normal) * i.normal;
    return  Ray(i.position, dir);
} 

Ray refract(const Ray r, const Intersection i)
{
    vec3 idir = vec3(r.direction.x,
            r.direction.y,
            r.direction.z);
    vec3 norm = vec3(i.normal.x,
            i.normal.y,
            i.normal.z);

    float cosi = glm::clamp(glm::dot(idir, norm),- 1.f, 1.f );
    float etai = 1.f, etat = i.ior;
    vec4 n = i.normal;
    if (cosi < 0) { cosi = - cosi;} 
    else{ std::swap(etai, etat); n = -n;  }
    float eta = etai/etat;
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    vec3 dir =  k < 0.f ? vec3(0) : eta * idir + (eta * cosi - glm::sqrt(k)) * norm;  
    vec4 dir_end = vec4(dir.x,dir.y,dir.z, 1);
    return Ray(i.position, dir_end);
}

void fresnel(const Ray r, const Intersection i, float &kr)
{
    vec3 dir = glm::normalize(vec3(r.direction[0], r.direction[1], r.direction[2]));
    vec3 norm = vec3(i.normal[0], i.normal[1], i.normal[2]);
    float cosi = glm::clamp(glm::dot(dir, norm), -1.f, 1.f);
    float etai = 1, etat = i.ior;
    if (cosi > 0) {  std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * glm::sqrt(glm::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else
    {
        float cost = glm::sqrt(glm::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

void shootRay(const Ray r, vec3 &colour, BVH bvh, Light light)
{
    float refl_color, refr_color;
    vec3 lightColor;
    float kr;
    Ray r1, r2;

    stack<Ray_t> ray_stack = stack<Ray_t>();
    Ray_t c_ray;

    Intersection intersect = {
                vec4(0),
                vec3(0),
                std::numeric_limits<float>::max(),
                vec4(0)};

    ray_stack.push(Ray_t(r,0,1.0f));
    colour = vec3(0);
    while (!ray_stack.empty())
    {
        c_ray = ray_stack.top();
        ray_stack.pop();
        if (collision(bvh,
                    c_ray.r, intersect))
        {
            fresnel(c_ray.r, intersect, kr);
            lightColor = DirectLight(intersect,
                    bvh,
                    light);

            if (kr < 1)
            {
                r1 = refract(c_ray.r, intersect);
                refr_color = (float) intersect.refract_ratio;
            }

            r2 = reflect(c_ray.r, intersect);
            refl_color = intersect.reflect_ratio;

            colour += (refl_color * kr + refr_color * (1-kr))
                * lightColor
                * intersect.colour
                * c_ray.prevReflectance;
            
            if (c_ray.depth + 1 < RAY_DEPTH)
            {
                ray_stack.push(Ray_t(r1,
                            c_ray.depth + 1,
                            c_ray.prevReflectance *(1- intersect.refract_ratio)));
                //ray_stack.push(Ray_t(r2, c_ray.depth+1, c_ray.prevReflectance));
            }
        }
    }
}
