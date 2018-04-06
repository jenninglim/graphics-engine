#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define DEG_TO_RAD(deg) \
    deg * 3.14159 / 180.f

#define RAD_TO_DEG(rad) \
    180.f * rad / 3.14159

// Find A Orthogonal unit vector
vec3 findOthor(const vec3 vector);

#endif 
