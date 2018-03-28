#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <vector>
#include "Object.h"
#include "BVH.h"
#include "Light.h"
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "SDLauxiliary.h"

typedef const uint8_t KeyStroke_t;

class Scene
{
    Camera cam;
    Light light;
    vector<Object *> objects;
    BVH bvh;
    public:
        Scene();
        void Update(KeyStroke_t * keystate);
        void Draw(screen* screen);
};

#endif
