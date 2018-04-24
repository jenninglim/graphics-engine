#include "Config.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"

using glm::vec3;
using glm::vec4;

using namespace std;

class Box : public Object
{
public:
	Box(vector<Triangle>& tri) : Object(tri)
	{
		ScaleBox(triangles);
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
	    DrawPolygonRasterisation(screen, vertices, triangles[i].color, cam, light, currentNormal, currentReflectance, Draw::SHADOWS_OFF);
	  }
	}

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
			vec4 currentNormal = glm::normalize(shadowVolume[i].normal);
			vec3 currentReflectance = vec3(1,1,1);
			vector<Vertex> vertices(3);
			vertices[0].position = shadowVolume[i].v0;
			vertices[1].position = shadowVolume[i].v1;
			vertices[2].position = shadowVolume[i].v2;
			DrawPolygonRasterisation(screen, vertices, shadowVolume[i].color, cam, light, currentNormal, currentReflectance, Draw::SHADOW);
		}
	}

private:
	void ScaleBox(vector<Triangle>& triangles){
		//std::cout << triangles[0].v0 << endl;
	  for( size_t i=0; i<triangles.size(); ++i )
	    {
		    triangles[i].v0 *= 2/(float)L;
		    triangles[i].v1 *= 2/(float)L;
		    triangles[i].v2 *= 2/(float)L;

		    triangles[i].v0 -= vec4(1,1,1,1);
		    triangles[i].v1 -= vec4(1,1,1,1);
		    triangles[i].v2 -= vec4(1,1,1,1);

		    triangles[i].v0.x *= -1;
		    triangles[i].v1.x *= -1;
		    triangles[i].v2.x *= -1;

		    triangles[i].v0.y *= -1;
		    triangles[i].v1.y *= -1;
		    triangles[i].v2.y *= -1;

		    triangles[i].v0.w = 1.0;
		    triangles[i].v1.w = 1.0;
		    triangles[i].v2.w = 1.0;
		    triangles[i].ComputeNormal();
		}
	}
};
