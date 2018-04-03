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
		BunnyRotate(triangles, 2.3);
		BunnyTranslate(triangles, vec4(0.3,0.4,-0.5,0));
	}

public:
	void DrawPolygonAmbient(screen* screen, Camera* cam, Light* light){
		for(uint32_t i=0; i<triangles.size(); ++i){

			vec4 currentNormal = glm::normalize(triangles[i].normal);
	    vec3 currentReflectance = vec3(1,1,1);
	    vector<Vertex> vertices(3);
	    vertices[0].position = triangles[i].v0;
	    vertices[1].position = triangles[i].v1;
	    vertices[2].position = triangles[i].v2;
	    DrawPolygonRasterisation(screen, vertices, triangles[i].color, cam, light, currentNormal, currentReflectance, Draw::SCENE_AMBIENT);
	  }
	}

	void DrawPolygonShadow(screen* screen, Camera* cam, Light* light){
		for(uint32_t i=0; i<triangles.size(); ++i){

			vec4 currentNormal = glm::normalize(triangles[i].normal);
			vec3 currentReflectance = vec3(1,1,1);
			vector<Vertex> vertices(3);
			vertices[0].position = triangles[i].v0;
			vertices[1].position = triangles[i].v1;
			vertices[2].position = triangles[i].v2;
			DrawPolygonRasterisation(screen, vertices, triangles[i].color, cam, light, currentNormal, currentReflectance, Draw::SCENE_SHADOW);
		}
	}

	void DrawShadowVolume(screen *screen, Camera* cam, Light* light){
		for(uint32_t i=0; i<shadowVolume.size(); ++i){
			vec4 currentNormal = glm::normalize(triangles[i].normal);
			vec3 currentReflectance = vec3(1,1,1);
			vector<Vertex> vertices(3);
			vertices[0].position = triangles[i].v0;
			vertices[1].position = triangles[i].v1;
			vertices[2].position = triangles[i].v2;
			DrawPolygonRasterisation(screen, vertices, shadowVolume[i].color, cam, light, currentNormal, currentReflectance, Draw::SCENE_AMBIENT);
		}
	}

	void BunnyRotate(vector<Triangle> &triangles, float radians){
		for(size_t i = 0; i < triangles.size(); i++){
			triangles[i].RotateXAxis(radians);
		}
	}

	void BunnyTranslate(vector<Triangle> &triangles, vec4 translation){
		for(size_t i = 0; i < triangles.size(); i++){
			triangles[i].Translate(translation);
		}
	}
};
