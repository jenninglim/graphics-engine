#include "Ray.h"
#include "Config.h"
#include "BVH.h"

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
    float etat = 1.f, etai = i.refract;
    vec4 n = i.normal;
    if (cosi < 0) { cosi = - cosi;} 
    else{ std::swap(etai, etat);n = -n; }
    float eta = etai/etat;
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    vec3 dir =  k < 0.f ? vec3(0) : eta * idir + (eta * cosi - glm::sqrt(k)) * norm;  
    vec4 dir_end = vec4(dir.x,dir.y,dir.z, 1);
    return Ray(i.position, dir_end);
}

void fresnel(const Ray r, const Intersection i, float &kr)
{
    float cosi = glm::clamp(glm::dot(r.direction, i.normal), -1.f, 1.f);
    float etai = 1, etat = i.refract;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

void shootRay(Ray r, Intersection &intersect, vec3 &colour, BVH bvh, Light light)
{
    vec3 refl_color, refr_color, lightColor;
    float prevReflectance = 1;
    float kr;
    for (int i=0; i < RAY_DEPTH; i++)
    {
        if (collision(bvh,
                    r.initial,
                    r.direction, intersect))
        {
            //fresnel(r, closestIntersection, kr);
            r = reflect(r, intersect);

            /*
               if (kr < 1)
               {
               r1 = refract(r, closestIntersection);
               collision(bvh,r,closestIntersection);
               refr_color = prevReflectance
             * (float) closestIntersection.reflect
             * closestIntersection.colour
             * lightColor;
             }
             */

            lightColor = DirectLight(intersect,
                    bvh,
                    light);

            refl_color = prevReflectance
                * (float) intersect.reflect
                * intersect.colour
                * lightColor;

            prevReflectance *= (1 - intersect.reflect);

            colour += refl_color;

        }
        else
        {
            break;
        }

    }

}
