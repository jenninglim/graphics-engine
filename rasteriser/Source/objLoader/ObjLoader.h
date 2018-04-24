#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;
using glm::vec4;
using glm::vec3;

bool loadObj(
        const char * path,
        vector<vec4> & out_vector,
        vector<vec3> & out_norm
        );

#endif
