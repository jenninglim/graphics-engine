#include "Triangle.h"

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

