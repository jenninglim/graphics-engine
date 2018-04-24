#include "Scene.h"

using namespace std;
using glm::vec3;


void drawPixels(screen * screen, Camera* cam);

Scene::Scene(){
    this->cam = new Camera();
    this->light = new Light();
    //LoadCornellBox(this->cornellBox);
    LoadObjects(this->objects);
  }

void Scene::Update(const uint8_t* keystate){
    if ( keystate[SDL_SCANCODE_UP] )
    {
  	  cam->forward();
    }
    if ( keystate[SDL_SCANCODE_DOWN] )
    {
  	  cam->backward();
    }
    if ( keystate[SDL_SCANCODE_LEFT] )
    {
  	  cam->left();
    }
    if ( keystate[SDL_SCANCODE_RIGHT] )
    {
  	  cam->right();
    }

    if ( keystate[SDL_SCANCODE_W] )
    {
  	  light->forward();
    }
    if ( keystate[SDL_SCANCODE_S] )
    {
  	  light->backward();
    }
    if ( keystate[SDL_SCANCODE_A] )
    {
  	  light->left();
    }
    if ( keystate[SDL_SCANCODE_D] )
    {
  	  light->right();
    }
    if ( keystate[SDL_SCANCODE_Q] )
    {
      light->down();
    }
    if ( keystate[SDL_SCANCODE_E] )
    {
      light->up();
    }
}

void Scene::Draw(screen* screen){

  for (int i = 0 ; i < SCREEN_HEIGHT; i++)
  {
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
      cam->depthBuffer[i][j] = 0.0f;
      cam->pixels[i][j] = vec3(0);
    }
  }
  //DRAW THE POLYGONS
  for(size_t i = 0; i<objects.size(); i++){
    //FIRST IS CORNELL BOX
    objects[i]->DrawPolygon(screen,cam,light);
  }
  drawPixels(screen, cam);

}

void drawPixels(screen * screen, Camera* cam){
  for(int i = 0; i< SCREEN_HEIGHT; i++){
    for(int j = 0; j < SCREEN_WIDTH; j++){
      PutPixelSDL(screen,j,i,cam->pixels[i][j]);
    }
  }
}
