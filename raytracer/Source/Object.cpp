#include <limits>
#include <glm/glm.hpp>
#include "Object.h"

#include <iostream>
#include "glm/ext.hpp"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

void scale_volume(vector<Triangle> &triangles);
vec3 triangleMin(Triangle triangle);
vec3 triangleMax(Triangle triangle);

Object::Object()
{

}

Object::Object(vector<Triangle> object)
{
    scale_volume(object);
    triangles = object;
    computeBoundingVolume();
    this->reflect = 0.6;
    this->refract= 0;
}

Object::Object(vector<Triangle> object, double reflectance)
{
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect = reflectance;
    this->refract= 1;
}

Object::Object(vector<Triangle> object, double reflectance, double refract_index)
{
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect = reflectance;
    this->refract= refract_index;
}


BoundingVolume::BoundingVolume()
{
    this->max = vec3(std::numeric_limits<float>::max());
    this->min = vec3(std::numeric_limits<float>::min());
}

BoundingVolume::BoundingVolume(vec3 min, vec3 max)
{
    this->min = min;
    this->max = max;
}

void Object::computeBoundingVolume()
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

vec3 triangleMin(Triangle triangle)
{
    vec3 min = vec3(std::numeric_limits<float>::max());
    if (triangle.v0.x < min.x) { min.x = triangle.v0.x; }
    if (triangle.v0.y < min.y) { min.y = triangle.v0.y; }
    if (triangle.v0.z < min.z) { min.z = triangle.v0.z; }

    if (triangle.v1.x < min.x) { min.x = triangle.v1.x; }
    if (triangle.v1.y < min.y) { min.y = triangle.v1.y; }
    if (triangle.v1.z < min.z) { min.z = triangle.v1.z; }

    if (triangle.v2.x < min.x) { min.x = triangle.v2.x; }
    if (triangle.v2.y < min.y) { min.y = triangle.v2.y; }
    if (triangle.v2.z < min.z) { min.z = triangle.v2.z; }
    return min;
}

vec3 triangleMax(Triangle triangle)
{
    vec3 max = vec3(std::numeric_limits<float>::min());
    if (triangle.v0.x > max.x) { max.x = triangle.v0.x; }
    if (triangle.v0.y > max.y) { max.y = triangle.v0.y; }
    if (triangle.v0.z > max.z) { max.z = triangle.v0.z; }

    if (triangle.v1.x > max.x) { max.x = triangle.v1.x; }
    if (triangle.v1.y > max.y) { max.y = triangle.v1.y; }
    if (triangle.v1.z > max.z) { max.z = triangle.v1.z; }

    if (triangle.v2.x > max.x) { max.x = triangle.v2.x; }
    if (triangle.v2.y > max.y) { max.y = triangle.v2.y; }
    if (triangle.v2.z > max.z) { max.z = triangle.v2.z; }
    return max;
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
