#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Config.h"
#include "BoundingVolume.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


class Object
{
    public:
        BoundingVolume bv;
        float reflect_ratio;
        float refract_ratio;
        float ior;
        virtual ~Object() {};
        virtual bool intersection(const Ray r, Intersection &i)
        {
            cout << "intersection virtual called" <<endl;
            assert(0 > 1);
        };
        virtual bool intersection(const Cone r, Intersection &i)
        {
            cout << "intersection virtual called" <<endl;
            assert(0 > 1);
        };

};
#endif 

