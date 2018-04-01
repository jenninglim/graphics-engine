#include "Sphere.h"
#include "Config.h"
#include <glm/glm.hpp>

#ifdef DEBUG
#include <iostream>
#include "glm/ext.hpp"
#endif

using namespace glm;

double circcircArea(vec2 pos1, float r1,
                   vec2 pos2, float r2);
vec2 projection(vec3 point, vec3 e1, vec3 e2);
vec3 ComputeNormal(vec3 v1, vec3 v2);

Sphere :: Sphere(const vec4 c, const float r)
{
    this->centre = c;
    this->radius = r;
    this->reflect_ratio = SPHERE_REFLECTANCE;
    this->refract_ratio = SPHERE_REFRACT;
    this->ior = 1.;
    this->colour = vec3(0.5,1.0f,1.0f);
}

bool Sphere::intersection(const Ray r, Intersection &closestI)
{
    bool intersectionFound = true;
    vec3 cent = vec3(this->centre);
    vec3 dir = vec3(r.direction);
    vec3 init = vec3(r.initial);
    float t, t0, t1;

    // analytic solution
    vec3 L = cent-init;
    float tca = glm::dot(L, dir); 
    float d2 = glm::dot(L,L) - tca*tca;
    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (d2 > this->radius*this->radius || tca < 0.0f)
    {
        intersectionFound = false;
    }
    float thc = glm::sqrt(this->radius*this->radius - d2);
    // A negative discriminant corresponds to ray missing sphere
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) std::swap(t0, t1); 

    if (t0 < -EPSILON) { 
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < -EPSILON) return false; // both t0 and t1 are negative 
    } 
    t = t0; 

    if (t > EPSILON && t < closestI.distance)
    {
        closestI.position = t * vec4(r.direction,0) + r.initial;
        closestI.distance = t;
        closestI.colour = this->colour;
        closestI.normal = glm::normalize(closestI.position - this->centre);
        closestI.reflect_ratio = this->reflect_ratio;
        closestI.refract_ratio = this->refract_ratio;
        closestI.ior = this->ior;
    }
    return intersectionFound;
}

bool Sphere::intersection(const Cone r, Intersection &closestI)
{
    vec3 AB = vec3(this->centre - r.initial);
    float T = glm::dot(AB, r.direction) / glm::dot(r.direction,r.direction);
    vec3 closestPoint = vec3(r.initial) +  T * r.direction;
    float d = T * glm::tan(r.theta) + this->radius/glm::cos(r.theta);
    float SEP = glm::l2Norm(closestPoint -  vec3(this->centre));
    if (d > SEP) 
    {
        if (closestI.distance > T)
        {
            closestI.distance = T;
        }
        double area = 0;
        double r1, r2;

        vec3 v1 = glm::normalize(closestPoint - vec3(this->centre));
        vec3 v2 = glm::normalize(ComputeNormal(v1, r.direction));

        vec2 proj_circ1;
        vec2 proj_circ2;
        float radius_max;
        
        //determine v1 and v2
        proj_circ1 = projection(vec3(r.initial), v1,v2);
        proj_circ2 = projection(vec3(this->centre), v1,v2);
        closestI.area = 0.f;
        for (double i = 0; i < this->radius * 2; i += VOL_APPROX_INT)
        {
            r1 = glm::tan(r.theta) * (T + i - this->radius);
            r2 = this->radius * glm::sin(glm::acos(glm::abs(this->radius - i)/this->radius));
            area = circcircArea(proj_circ1, r1, proj_circ2, r2);
            radius_max = glm::pi<float>() * glm::max<float>(r1 * r1, r2 * r2);
            closestI.area = glm::max<float>(closestI.area, area / radius_max);
        }
        return true;
    }
    return false;
}

vec2 projection(vec3 point, vec3 e1, vec3 e2)
{
    float v1 = glm::dot(e1,point) / glm::dot(e1, e1);
    float v2 = glm::dot(e2,point) / glm::dot(e2, e2);
    return vec2(v1,v2);
}

double circcircArea(vec2 pos1, float r1,
                   vec2 pos2, float r2)
{
    float d = glm::length(pos1 - pos2);
    if (r1 < 0 || r2 < 0)
    {
        return 0.;
    }
    if (d < glm::abs(r2 - r1))
    {
        return glm::pi<float>() * glm::min(r1 * r1, r2 * r2);
    }
    if (d < r1 + r2)
    {
       float a = r1 * r1;
       float b = r2 * r2;

       float x =(a - b + d * d) / (2 * d); 
       float y =(d * d - a + b) / (2 * d); 
       float z = (-d + r1 +r2) * (d + r1 - r2) * (d - r1 + r2) * (d + r1 + r2);
      
       return a * glm::acos(x / r1)
            + b * glm::acos(y / r2)
            - 0.5f * (glm::sqrt(z));
    }
    return 0.f;
}

vec3 ComputeNormal(vec3 v1, vec3 v2)
{
    vec3 normal;
    glm::vec3 normal3 = glm::normalize( glm::cross( v2, v1 ) );
    normal.x = normal3.x;
    normal.y = normal3.y;
    normal.z = normal3.z;
    return normal;
}
