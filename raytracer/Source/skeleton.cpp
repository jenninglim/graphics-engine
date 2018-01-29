#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include <stdint.h>
#include "glm/ext.hpp"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define CAM_FOCAL_LENGTH 400 
/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(screen* screen, Camera cam, vector<Triangle>& triangles);
bool ClosestIntersection(vec4 start, vec4 dir, const vector<Triangle> &triangles, Intersection &closestIntersection);

int main( int argc, char* argv[] )
{
  
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  vector<Triangle> triangles;
  vec4 camPos(0,0,-2,1);
  Camera cam(CAM_FOCAL_LENGTH, camPos);
  LoadTestModel(triangles);
  
  while( NoQuitMessageSDL() )
    {
      Update();
      Draw(screen, cam, triangles);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, Camera cam, vector<Triangle>& triangles)
{
    /* Clear buffer */
     memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            vec4 d(x - SCREEN_WIDTH/2, y- SCREEN_HEIGHT/2, cam.focalLength, 1) ;
            Intersection closestIntersection = {
                cam.cameraPos,
                std::numeric_limits<float>::max(),
                0};
            if (ClosestIntersection(cam.cameraPos, d, triangles, closestIntersection))
            {
                vec3 color =triangles[closestIntersection.triangleIndex].color;
                //cout << closestIntersection.triangleIndex <<endl;
                PutPixelSDL(screen, x, y, color);
            }
        }
    }
    
    /*
  uint32_t x = rand() % screen->width;
      uint32_t y = rand() % screen->height;
      PutPixelSDL(screen, x, y, colour);
    }
    */
}


/*Place updates of parameters here*/
void Update()
{
  static int t = SDL_GetTicks();
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;
  /*Good idea to remove this*/
  std::cout << "Render time: " << dt << " ms." << std::endl;
  /* Update variables*/
}

vec3 solveLinearEq(Triangle triangle, Ray r)
{
    vec4 v0 = triangle.v0;
    vec4 v1 = triangle.v1;
    vec4 v2 = triangle.v2;
    vec3 s = glm::vec3(r.initial.x, r.initial.y, r.initial.z);
    vec3 d = glm::vec3(r.direction.x, r.direction.y, r.direction.z);

    vec3 e1 = vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
    vec3 e2 = vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
    vec3 b = vec3(s.x-v0.x,s.y-v0.y,s.z-v0.z);
    
    mat3 A( -d, e1, e2 );
    return glm::inverse( A ) * b;
}

bool ClosestIntersection(vec4 start, vec4 dir, const vector<Triangle> &triangles, Intersection &closestIntersection){
    
    bool intersectionFound =  false;
    Ray ray(start, dir);
    for(int i = 0; i < triangles.size(); i++){
        vec3 x_value = solveLinearEq(triangles[i],ray);
        if(x_value.y > 0 && x_value.z > 0 && x_value.y + x_value.z < 1 && x_value.x >= 0){
            //Valid Intersection found
            intersectionFound = true;
            if(x_value.x < closestIntersection.distance){
                closestIntersection.position = start + x_value.x * dir;
                closestIntersection.distance = x_value.x;
                closestIntersection.triangleIndex = i;
            }
        }
    }
    return intersectionFound;
}
