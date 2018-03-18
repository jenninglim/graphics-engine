#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <stdint.h>
#include "glm/ext.hpp"
#include <math.h>
#include "Scene.h"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define _USE_MATH_DEFINES

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update(Scene &scene);

void Draw(screen* screen,
        Scene scene);

int main( int argc, char* argv[] )
{
    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

    Scene scene = Scene();
    while( NoQuitMessageSDL() )
    {
        Update(scene);
        Draw(screen, scene);
        SDL_Renderframe(screen);
    }

    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
    return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, Scene scene)
{
   /* Clear buffer */
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
    scene.Draw(screen);
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
    /* Update variables*/
    KeyStroke_t * keystate = SDL_GetKeyboardState( 0 );
    scene.Update(keystate);
}
