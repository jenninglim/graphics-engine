#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include "Triangle.h"
#include "Intersection.h"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

class BoundingVolume
{
    public:
        vec3 min;
        vec3 max;
        BoundingVolume();
        BoundingVolume(vec3 min, vec3 max);
};

class Object
{
    public:
        vector<Triangle> triangles;
        BoundingVolume bv;
        float reflect_ratio;
        float refract_ratio;
        float ior;
        Object();
        Object(vector<Triangle> object);
        Object(vector<Triangle> object, float reflectance);
        Object(vector<Triangle> object, float reflectance, float refract_index );
        void computeBoundingVolume();
        //Intersection intersection(
};
#endif 
