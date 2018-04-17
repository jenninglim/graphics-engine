#include "Box.h"
#include "Config.h"
#include <limits>
#include "Overlap.h"

#ifdef DEBUG
#include <iostream>
#include <glm/ext.hpp>
#endif

void scale_volume(vector<Triangle> &triangles);

Box::Box()
{
    this->reflect_ratio = 0.f;
    this->ior= DEF_IOR;
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object)
{
    this->colour = object[0].color;
    scale_volume(object);
    triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = 0.6;
    this->ior= DEF_IOR;
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object, float reflectance)
{
this->colour = object[0].color;
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = reflectance;
    this->ior= DEF_IOR; 
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object, float reflectance, float refract_index)
{
this->colour = object[0].color;
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = reflectance;
    this->refract_ratio= BOX_REFRACT;
    this->ior = refract_index;
}

bool Box::intersection(const Ray r, Intersection &closestI) 
{
    bool intersectionFound = false;
    vec3 e1,e2, s, h, q;
    float a,f,u,v,t;

    for (int i=0; i < triangles.size(); i++)
    {
        e1 = vec3(triangles[i].v1 - triangles[i].v0);
        e2 = vec3(triangles[i].v2 - triangles[i].v0);
        h = glm::cross(e2, r.direction);
        a = glm::dot(h, e1);
        
        f = 1/a;
        s = r.initial - vec3(triangles[i].v0);
        u = f * (glm::dot(s,h));
        
        q = glm::cross(e1, s);
        v = f * glm::dot(r.direction, q);

        if (((v > 0.f -EPSILON && u + v < 1.f + EPSILON)
            && (u > 0.f - EPSILON && u < 1.f + EPSILON))
            && (a < -EPSILON || a > EPSILON))
        {
            t = f * glm::dot(e2,q); 
            if (t>EPSILON && t < closestI.distance)
            {
                intersectionFound = true;

                closestI.position = vec4(r.initial,0) + vec4(t * r.direction,0);
                closestI.distance = t;
                closestI.colour = triangles[i].color;
                closestI.normal = triangles[i].normal;
                closestI.reflect_ratio = this->reflect_ratio;
                closestI.refract_ratio = this->refract_ratio;
                closestI.ior = this->ior;
            }
        }
    }
    return intersectionFound;
}
bool Box::intersection(const Cone r, Intersection &i)
{
    return false;
}

void Box::computeBoundingVolume()
{
    vec3 max = vec3(std::numeric_limits<float>::min());

    vec3 min = vec3(std::numeric_limits<float>::max());

    vec3 tri_min;
    vec3 tri_max;
    for (int i = 0; i < triangles.size(); i++)
    {
        tri_min = triangleMin(triangles[i]);
        tri_max = triangleMax(triangles[i]);
          
        if (tri_min.x < min.x) { min.x=tri_min.x; }
        if (tri_min.y < min.y) { min.y=tri_min.y; }
        if (tri_min.z < min.z) { min.z=tri_min.z; }
        if (tri_max.x > max.x) { max.x=tri_max.x; }
        if (tri_max.y > max.y) { max.y=tri_max.y; }
        if (tri_max.z > max.z) { max.z=tri_max.z; }
    }
    bv = BoundingVolume(min, max);
}

bool Box::boxOverlap(vec3 boxcenter, vec3 boxhalfsize, Intersection &inter)
{
   for (int i = 0; i <  triangles.size(); i++)
   {
        if (trigBoxOverlap(boxcenter, boxhalfsize, this->triangles[i]))
        {
            inter.colour = this->triangles[i].color;
            inter.normal = this->triangles[i].normal; 
            return true;
        }
   }
   return false;
}

void scale_volume(vector<Triangle> &triangles)
{
    float L = 555;
    	// ----------------------------------------------
	// Scale to the volume [-1,1]^3

	for( size_t i=0; i<triangles.size(); ++i )
	{
		triangles[i].v0 *= 2/L;
		triangles[i].v1 *= 2/L;
		triangles[i].v2 *= 2/L;

		triangles[i].v0 -= vec4(1,1,1,1);
		triangles[i].v1 -= vec4(1,1,1,1);
		triangles[i].v2 -= vec4(1,1,1,1);

		triangles[i].v0.x *= -1;
		triangles[i].v1.x *= -1;
		triangles[i].v2.x *= -1;

		triangles[i].v0.y *= -1;
		triangles[i].v1.y *= -1;
		triangles[i].v2.y *= -1;

		triangles[i].v0.w = 1.0;
		triangles[i].v1.w = 1.0;
		triangles[i].v2.w = 1.0;

		triangles[i].ComputeNormal();
	}

}
