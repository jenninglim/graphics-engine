#include "Config.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"


using glm::vec3;
using glm::vec4;

using namespace std;

class Edge
{
public:
  vec4 vertex1;
  vec4 vertex2;

	Edge(vec4 v1, vec4 v2) : vertex1(v1), vertex2(v2)
	{

	}

  //COMPARE IF EDGES ARE THE SAME 
  bool CompareEdges(Edge &e) const{
    return ((vertex1 == e.vertex1 && vertex2 == e.vertex2) || (vertex1 == e.vertex2 && vertex2 == e.vertex1));
  }
};
