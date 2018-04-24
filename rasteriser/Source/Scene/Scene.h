#include "Camera.h"
#include "Light.h"
#include "TestModelH.h"
#include <SDL.h>
#include "Post.h"
#include "Edge.h"
#include <algorithm>


class Scene
{
  Camera* cam;
  Light* light;
  //Object * cornellBox;
  vector<Object *> objects;

public:
  Scene();
  void Update(const uint8_t* keystate);
  void Draw(screen* screen);
  void DrawPolygonsAmbient(screen* screen);
  void CreateShadowVolumes();
  void DrawShadowVolumes(screen *screen);
  void DrawPolygonShadows(screen *screen);
};
