#include "Config.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"

using glm::vec3;
using glm::vec4;

using namespace std;

class Bunny : public Object
{
public:
	Bunny(vector<Triangle>& tri) : Object(tri)
	{
		Rotate(triangles, 2.3);
	}

public:
	void DrawPolygon(screen* screen, Camera* cam, Light* light){
		for(uint32_t i=0; i<triangles.size(); ++i){

			vec4 currentNormal = glm::normalize(triangles[i].normal);
	    vec3 currentReflectance = vec3(1,1,1);
	    vector<Vertex> vertices(3);
	    vertices[0].position = triangles[i].v0;
	    vertices[1].position = triangles[i].v1;
	    vertices[2].position = triangles[i].v2;
	    DrawPolygonRasterisation(screen, vertices, triangles[i].color, cam, light, currentNormal, currentReflectance);
	  }
	}

	void Rotate(vector<Triangle> &triangles, float radians){
		for(size_t i = 0; i < triangles.size(); i++){
			//std::cout<<glm::to_string(triangles[i].v0)<<std::endl;
			triangles[i].RotateXAxis(radians);
			//std::cout<<glm::to_string(triangles[i].v0)<<std::endl;
		}
	}
};
