#include "Triangle.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"

using namespace std;

class Object
{
public:
  vector<Triangle> triangles;
  vector<Triangle> shadowVolume;

public:
  Object(vector<Triangle> triangles) : triangles(triangles)
  {
  }

public:
  virtual void DrawPolygon(screen* screen, Camera *cam, Light* light){
    std::cout << "Test Object Draw" << std::endl;
  };
  virtual void DrawPolygonAmbient(screen* screen, Camera *cam, Light* light){
    std::cout << "Test Object Draw" << std::endl;
  };

  virtual void DrawShadowVolume(screen* screen, Camera *cam, Light* light){
      std::cout << "Test Object Shadow Volume" << std::endl;
  }

  virtual void DrawPolygonShadow(screen* screen, Camera *cam, Light* light){
      std::cout << "Test Object Shadow Volume" << std::endl;
  }

  //ROTATE OBJECT AROUND X AXIS BY GIVEN RADIANS
	void RotateX(vector<Triangle> &triangles, float radians){
		for(size_t i = 0; i < triangles.size(); i++){
			triangles[i].RotateXAxis(radians);
		}
	}

  //TRANSLATE OBJECT
	void Translate(vector<Triangle> &triangles, vec4 translation){
		for(size_t i = 0; i < triangles.size(); i++){
			triangles[i].Translate(translation);
		}
	}

};
