#include <iostream>
#include <stdint.h>

#include <glm/glm.hpp>
#include "Scene.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

/*
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT SCREEN_WIDTH
#define CAM_FOCAL_LENGTH SCREEN_WIDTH
#define FULLSCREEN_MODE false
#define EPSILON 1e-2
*/
/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update(Scene &scene);
void Draw(screen* screen, Scene &scene);

int main( int argc, char* argv[] )
{
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  Scene scene = Scene();
  while( NoQuitMessageSDL() )
    {
      Update(scene);
      Draw(screen,scene);
      SDL_Renderframe(screen);
    }
  SDL_SaveImage( screen, "screenshot.bmp" );
  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, Scene &scene)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  scene.Draw(screen);
  /*
  float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
  for (int i = 0 ; i < SCREEN_HEIGHT; i++)
  {
      for (int j = 0; j < SCREEN_WIDTH; j++)
      {
          depthBuffer[i][j] = 0.0f;
      }
  }

  for(uint32_t i=0; i<triangles.size(); ++i){
    vec4 currentNormal = glm::normalize(triangles[i].normal);

    vec3 currentReflectance = vec3(1,1,1);
    vector<Vertex> vertices(3);
    vertices[0].position = triangles[i].v0;
    vertices[1].position = triangles[i].v1;

    vertices[2].position = triangles[i].v2;
    //DrawPolygon(screen, vertices, triangles[i].color, cam, depthBuffer, light, currentNormal, currentReflectance);
  }
  */
}

/*Place updates of parameters here*/
void Update(Scene &scene)
{
  static int t = SDL_GetTicks();
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;
  /*Good idea to remove this*/
  std::cout << "Render time: " << dt << " ms." << std::endl;
  scene.Update(SDL_GetKeyboardState( 0 ));
}
