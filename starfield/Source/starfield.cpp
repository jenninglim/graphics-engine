#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include <stdint.h>
#include <math.h>

using namespace std;
using glm::vec3;
using glm::vec2;
using glm::mat3;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define NUMBER_OF_STARS 10000
#define FOCAL_LENGTH SCREEN_HEIGHT / 2 
#define VELOCITY 0.001


/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
int t;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update(vector<vec3> & stars);
void Draw(screen* screen, vector<vec3> stars);
void Interpolate(vec3 a, vec3 b, vector<vec3> & result);
vec2 projection(vec3 coord);

int main( int argc, char* argv[] )
{
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );
  t = SDL_GetTicks();	/*Set start value for timer.*/
  
  vector<vec3> stars(NUMBER_OF_STARS);
  // initialise stars
  for (int i = 0; i < NUMBER_OF_STARS; i++)
  {
    float x = 2 * float(rand()) / float(RAND_MAX) -1;
    float y = 2 * float(rand()) / float(RAND_MAX) -1;
    float z = float(rand()) / float(RAND_MAX);
    stars[i] = vec3(x, y, z);
  }

  while( NoQuitMessageSDL() )
    {
      Draw(screen, stars);
      Update(stars);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );
  KillSDL(screen);

  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, vector<vec3> stars)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  
  vec3 colour;
  vec2 coord;
  for(int i=0; i<NUMBER_OF_STARS; i++)
  {
      colour = 0.2f * vec3(1,1,1) / (stars[i].z*stars[i].z);
      coord = projection(stars[i]);
      PutPixelSDL(screen, coord[0], coord[1], colour);
  }
}

/*Place updates of parameters here*/
void Update(vector<vec3> &stars)
{
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;
  /*Good idea to remove this*/
  std::cout << "Render time: " << dt << " ms." << std::endl;
  /* Update variables*/
  for (int i = 0; i < NUMBER_OF_STARS; i++)
  {
      if (stars[i].z <= 0)
      {
          stars[i].z += VELOCITY * dt * 10e-3;
      }
      else
      {
          stars[i].z -= VELOCITY * dt * 10e-3;
      }
      stars[i].z = fmod(stars[i].z + 1, 1);
  }
}

void Interpolate(vec3 a, vec3 b, vector<vec3>& result)
{
    vec3 step =  (b - a ) / ((float) result.size() - 1.0f);
    for (int i = 0; i < result.size(); i ++)
    {
        result[i] = a + step * (float) i;
    }
}

vec2 projection(vec3 coord)
{
    float u,v;
    u = FOCAL_LENGTH * coord.x / coord.z + SCREEN_WIDTH/2;
    v = FOCAL_LENGTH * coord.y / coord.z + SCREEN_HEIGHT/2;
    return vec2(u,v);
}
