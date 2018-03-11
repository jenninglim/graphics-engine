#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include "Camera.h"
#include <stdint.h>
#include "glm/ext.hpp"
#include <math.h>
#include "Light.h"
#include "Intersection.h"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define _USE_MATH_DEFINES
/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update(Camera &cam);

void Draw(screen* screen,
        Camera cam,
        vector<Object>& triangles,
        Light light);

int main( int argc, char* argv[] )
{
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  vector<Object> objects;
  Light light(vec4(0, -0.5, -0.7, 1), 14.f* vec3(1,1,1));

  vec4 camPos(0,0,-3,1);
  Camera cam(CAM_FOCAL_LENGTH, camPos);
  LoadTestModel(objects);

  while( NoQuitMessageSDL() )
    {
      Update(cam);
      Draw(screen, cam, objects, light);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, Camera cam, vector<Object>& objects, Light light)
{
    /* Clear buffer */
    //std::cout<<glm::to_string(cam.cameraPos)<<std::endl;
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
    Intersection closestIntersection = {
                cam.position,
                std::numeric_limits<float>::max(),
                0};

    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            vec4 rayFromOrigin(x - SCREEN_WIDTH/2,
                    y- SCREEN_HEIGHT/2,
                    cam.focalLength,1);
            vec4 rayFromCam = cam.R * rayFromOrigin;
            vec4 d(rayFromCam.x, rayFromCam.y, rayFromCam.z, 1) ;
            d = glm::normalize(d);
            closestIntersection.distance = std::numeric_limits<float>::max();
            if (ClosestIntersection(cam.position,
                        d,
                        objects,
                        closestIntersection))
            {
                vec3 lightColor = DirectLight(closestIntersection,
                        objects,
                        light);
                vec3 color = lightColor *
                    objects[closestIntersection.objectIndex].triangles[closestIntersection.triangleIndex].color;
                PutPixelSDL(screen, x, y, color);
            }
        }
    }
}

/*Place updates of parameters here*/
void Update(Camera &cam)
{
  static int t = SDL_GetTicks();
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;
  /*Good idea to remove this*/
  //std::cout << "Render time: " << dt << " ms." << std::endl;
  /* Update variables*/

  const uint8_t* keystate = SDL_GetKeyboardState( 0 );
	if ( keystate[SDL_SCANCODE_UP] )
	{
        cam.forward();
	}
	if ( keystate[SDL_SCANCODE_DOWN] )
	{
		cam.backward();
	}
	if ( keystate[SDL_SCANCODE_LEFT] )
	{
	    cam.left();
	}
	if ( keystate[SDL_SCANCODE_RIGHT] )
	{
	    cam.right ();
	}
}
