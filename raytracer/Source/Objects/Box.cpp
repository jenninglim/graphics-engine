#include "Box.h"
#include "Config.h"

#ifdef DEBUG
#include <iostream>
#endif

void scale_volume(vector<Triangle> &triangles);
vec3 triangleMin(Triangle triangle);
vec3 triangleMax(Triangle triangle);

Box::Box()
{
    this->reflect_ratio = 0.f;
    this->ior= DEF_IOR;
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object)
{
    scale_volume(object);
    triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = 0.6;
    this->ior= DEF_IOR;
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object, float reflectance)
{
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = reflectance;
    this->ior= DEF_IOR; 
    this->refract_ratio = 1.f;
}

Box::Box(vector<Triangle> object, float reflectance, float refract_index)
{
    scale_volume(object);
    this->triangles = object;
    computeBoundingVolume();
    this->reflect_ratio = reflectance;
    this->refract_ratio= DEF_REFRACT_RATIO;
    this->ior = refract_index;
}

vec3 solveLinearEq(Triangle triangle, vec4 start, vec4 dir)
{
    vec4 v0 = triangle.v0;
    vec4 v1 = triangle.v1;
    vec4 v2 = triangle.v2;
    vec3 s = glm::vec3(start.x, start.y, start.z);
    vec3 d = glm::vec3(dir.x, dir.y, dir.z);

    vec3 e1 = vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
    vec3 e2 = vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
    vec3 b = vec3(s.x-v0.x,s.y-v0.y,s.z-v0.z);

    mat3 A( -d, e1, e2 );
    //if (determinant(A) != 0)
    //{
        return glm::inverse( A ) * b;
    //}
    //return vec3(0,0,0);
}

bool Box::intersection(const Ray r, Intersection &closestI) 
{
    bool intersectionFound = false;
    vec3 x_value;
    vec4 dir = vec4(r.direction,0);
    for (int i=0; i < triangles.size(); i++)
    {
        x_value = solveLinearEq(triangles[i], r.initial, dir);
        if(x_value.y >= 0
                && x_value.z >= 0
                && x_value.y + x_value.z <= 1 + EPSILON
                && x_value.x > EPSILON)
        {
            //Valid Intersection found
            intersectionFound = true;
            if(x_value.x < closestI.distance){
                closestI.position = r.initial + x_value.x * dir;
                closestI.distance = x_value.x;
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
