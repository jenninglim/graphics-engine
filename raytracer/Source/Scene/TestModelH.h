#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H
// Defines a simple test model: The Cornel Box

#include "Config.h"
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"
#include "Triangle.h"

using namespace std;

// Helper function to push objects
void push_object(vector<Object> &objects, vector<Triangle> &object_tri);

void LoadTestModel( std::vector<Object>& objects );

#endif
