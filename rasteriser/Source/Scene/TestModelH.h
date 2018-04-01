#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H

// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>
#include "Config.h"
#include "Object.h"
#include "Box.h"
#include "ObjLoader.h"
#include "Bunny.h"

using glm::vec3;
using glm::vec4;

void push_box(std::vector<Object *> &objects, std::vector<Triangle> &object_tri);
void LoadCornellBox(Object * cornellBox);
void LoadObjects(std::vector<Object *>& objects );

#endif
