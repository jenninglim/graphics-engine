#ifndef MY_MATH_H
#define MY_MATH_H

#define DEG_TO_RAD(deg) \
    deg * 3.14159 / 180.f

#define RAD_TO_DEG(rad) \
    180.f * rad / 3.14159


#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
// Find A Orthogonal unit vector
vec3 findOthor(const vec3 v);
vec3 findOthor(const vec3 v1, const vec3 v2);
mat3 projMatr(const vec3 e1, const vec3 e2, const vec3 e3);

#endif 
