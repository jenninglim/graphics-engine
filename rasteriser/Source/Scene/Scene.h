#include "Camera.h"
#include "Light.h"
#include "TestModelH.h"
#include <SDL.h>
#include "Post.h"


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
};
