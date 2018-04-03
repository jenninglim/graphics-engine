#include "Triangle.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"
#include "Edge.h"

using namespace std;

class Object
{
public:
  vector<Triangle> triangles;
  vector<Triangle> shadowVolume;

public:
  Object(vector<Triangle>& triangles) : triangles(triangles)
  {
  }

public:
  virtual void DrawPolygonAmbient(screen* screen, Camera *cam, Light* light){
    std::cout << "Test Object Draw" << std::endl;
  };

  virtual void DrawShadowVolume(screen* screen, Camera *cam, Light* light){
      std::cout << "Test Object Shadow Volume" << std::endl;
  }

  virtual void DrawPolygonShadow(screen* screen, Camera *cam, Light* light){
      std::cout << "Test Object Shadow Volume" << std::endl;
  }

private:
  void ScaleBox(vector<Triangle>& triangles){
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
