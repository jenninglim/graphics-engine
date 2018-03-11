#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include "Triangle.h"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

class Object
{
    public:
        vector<Triangle> triangles;

    Object(vector<Triangle> object);
};

#endif 
