#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include "Camera.h"
#include <stdint.h>
#include "glm/ext.hpp"
#include <math.h>
#include "Light.h"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define _USE_MATH_DEFINES
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200
#define FULLSCREEN_MODE false
#define CAM_FOCAL_LENGTH 200
#define EPSILON 1e-3
/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update(Camera &cam);
void Draw(screen* screen, Camera cam, vector<Triangle>& triangles, Light light );
bool ClosestIntersection(vec4 start, vec4 dir, const vector<Triangle> &triangles, Intersection &closestIntersection);
vec3 DirectLight(const Intersection& i,vector<Triangle> triangles, Light light);

int main( int argc, char* argv[] )
{

  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  vector<Triangle> triangles;
  Light light(vec4(0, -0.5, -0.7, 1), 14.f* vec3(1,1,1));

  vec4 camPos(0,0,-2,1);
  Camera cam(CAM_FOCAL_LENGTH, camPos);
  LoadTestModel(triangles);

  while( NoQuitMessageSDL() )
    {
      Update(cam);
      Draw(screen, cam, triangles, light);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );


  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, Camera cam, vector<Triangle>& triangles, Light light)
{
    /* Clear buffer */
    std::cout<<glm::to_string(cam.cameraPos)<<std::endl;
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            vec4 rayFromOrigin(x - SCREEN_WIDTH/2, y- SCREEN_HEIGHT/2, cam.focalLength,1);
            vec4 rayFromCam = cam.R * rayFromOrigin;
            vec4 d(rayFromCam.x, rayFromCam.y, rayFromCam.z, 1) ;
            d = glm::normalize(d);
            Intersection closestIntersection = {
                cam.cameraPos,
                std::numeric_limits<float>::max(),
                0};
            if (ClosestIntersection(cam.cameraPos, d, triangles, closestIntersection))
            {
                vec3 lightColor = DirectLight(closestIntersection, triangles, light);
                vec3 color =lightColor * triangles[closestIntersection.triangleIndex].color;
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
  std::cout << "Render time: " << dt << " ms." << std::endl;
  /* Update variables*/

  const uint8_t* keystate = SDL_GetKeyboardState( 0 );
	if ( keystate[SDL_SCANCODE_UP] )
	{
        cam.forward();
        std::cout << "forward" << std::endl;
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
    //if (determinant(A) != 0)
    //{
        return glm::inverse( A ) * b;
    //}
    //return vec3(0,0,0);
}

bool ClosestIntersection(vec4 start, vec4 dir, const vector<Triangle> &triangles, Intersection &closestIntersection){

    bool intersectionFound =  false;
    Ray ray(start, dir);
    
    for(int i = 0; i < triangles.size(); i++){
        vec3 x_value = solveLinearEq(triangles[i],ray);
        if(x_value.y >= 0 && x_value.z >= 0 && x_value.y + x_value.z <= 1 && x_value.x > 0){
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

vec3 DirectLight(const Intersection& i, vector<Triangle> triangles, Light light)
{
    vec4 r_hat = glm::normalize(light.position - i.position);
    float dist = glm::length(light.position - i.position);
    vec4 n_hat = glm::normalize(triangles[i.triangleIndex].normal);

    vec3 lightColour = light.colour * glm::max(glm::dot(r_hat, n_hat), 0.0f) /
        (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));

    Intersection closestIntersection = {
                light.position,
                std::numeric_limits<float>::max(),
                0};

    if (ClosestIntersection(i.position, r_hat, triangles, closestIntersection))
    {
         if (closestIntersection.distance < glm::length(light.position - i.position) &&
                 closestIntersection.distance > EPSILON)
            {
                //cout << "distance" <<closestIntersection.distance<<endl;
                //cout << "dist" <<glm::length(light.position - i.position)<<endl;
                lightColour =  vec3(0,0,0);
            }
    }


    
    return lightColour;
}
