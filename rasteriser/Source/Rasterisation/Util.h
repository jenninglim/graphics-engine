#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
#include "SDLauxiliary.h"
#include <vector>
#include <Camera.h>
#include <Light.h>

enum Draw {SCENE_AMBIENT, SCENE_SHADOW, SHADOW};
typedef struct Pixel
{
    int x;
    int y;
    float zinv;
    glm::vec4 pos3d;
} Pixel;

typedef struct Vertex
{
    glm::vec4 position;
} Vertex;

void DrawPolygonRasterisation(screen* screen,
        const std::vector<Vertex>& vertices,
        vec3 color,
        Camera* cam,
        Light* light,
        vec4 currentNormal,
        vec3 currentReflectance,
        Draw type);
#endif
