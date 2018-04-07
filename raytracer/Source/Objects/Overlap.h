#ifndef OVERLAP_H
#define OVERLAP_H

#include "BoundingVolume.h"
#include "Triangle.h"

bool trigBoxOverlap(vec3 boxcenter, vec3 boxhalfsize, Triangle trig);

#endif 
