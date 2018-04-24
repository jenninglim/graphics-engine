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
		RotateX(triangles, 2.3);
		Translate(triangles, vec4(0.3,0.4,-0.5,0));
	}

public:
	//DRAW POLYGON WITHOUT SHADOWS
  void DrawPolygon(screen* screen, Camera* cam, Light* light){
    for(uint32_t i=0; i<triangles.size(); ++i){

      vec4 currentNormal = glm::normalize(triangles[i].normal);
      vec3 currentReflectance = vec3(1,1,1);
      vector<Vertex> vertices(3);
      vertices[0].position = triangles[i].v0;
      vertices[1].position = triangles[i].v1;
      vertices[2].position = triangles[i].v2;
      DrawPolygonRasterisation(screen, vertices, triangles[i].color, cam, light, currentNormal, currentReflectance, Draw::SHADOWS_OFF);
    }
  }

	//DRAW POLYGON WITH JUST AMBIENT LIGHTING
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
	
	//DRAW POLYGON BASED ON STENCIL BUFFER VALUES
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

	//DRAW SHADOW VOLUME MODELLING HARD SHADOWS
	void DrawShadowVolume(screen *screen, Camera* cam, Light* light){
		for(uint32_t i=0; i<shadowVolume.size(); ++i){
			vec4 currentNormal = glm::normalize(shadowVolume[i].normal);
			vec3 currentReflectance = vec3(1,1,1);
			vector<Vertex> vertices(3);
			vertices[0].position = shadowVolume[i].v0;
			vertices[1].position = shadowVolume[i].v1;
			vertices[2].position = shadowVolume[i].v2;
			DrawPolygonRasterisation(screen, vertices, shadowVolume[i].color, cam, light, currentNormal, currentReflectance, Draw::SHADOW);
		}
	}

};
