#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>
#include <vector>

#include "Object.h"
#include "Triangle.h"
#include "Ray.h"


class Box : public Object{
    public:
        vector<Triangle> triangles;

    Box();
    Box(vector<Triangle> object);
    Box(vector<Triangle> object, float reflectance);
    Box(vector<Triangle> object, float reflectance, float refract_index );
    void computeBoundingVolume() ;
    bool intersection(const Ray r, Intersection &i) ;
    bool intersection(const Cone r, Intersection &i);
};
#endif
