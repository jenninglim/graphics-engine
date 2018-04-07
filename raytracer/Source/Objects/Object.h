#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Intersection.h"
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
        vec3 colour;
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
        virtual bool boxOverlap(vec3 boxcenter, vec3 boxhalfisze, Intersection & i)
        {
            cout << "boxOverlap virtual called" << endl;
            assert(0 > 1);
        }
};

#endif 

