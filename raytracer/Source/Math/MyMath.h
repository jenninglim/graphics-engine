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

template<typename T>
T bilinear(const vec2 v,
        const T c00,
        const T c10,
        const T c01,
        const T c11)
{
    T a = c00 * (1.f - v.x) + c10 * v.x;
    T b = c01 * (1.f - v.x) + c11 * v.x;
    return a * (1.f - v.y) + b * v.y;
}

template<typename T>
T trilinear(const vec3 v,
        const T c000,
        const T c100,
        const T c010,
        const T c110,
        const T c001,
        const T c101,
        const T c011,
        const T c111)
{
    T e = bilinear<T>(vec2(v.x, v.y), c000, c100, c010, c110);
    T f = bilinear<T>(vec2(v.x, v.y), c001, c101, c011, c111);
    return e * (1-v.z) + f * v.z;
}

#endif 
