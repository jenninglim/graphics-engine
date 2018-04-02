#include "Scene.h"
#include <algorithm>

using namespace std;
using glm::vec3;

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
}

void Scene::Draw(screen* screen){

  for (int i = 0 ; i < SCREEN_HEIGHT; i++)
  {
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
      cam->depthBuffer[i][j] = 0.0f;
    }
  }
  //DRAW THE POLYGONS
  for(size_t i = 0; i<objects.size(); i++){
    //FIRST IS CORNELL BOX
    objects[i]->DrawPolygon(screen,cam,light);
    /*if(i != 0){
      //OBJECTS IN SCENE
      vector<Edge> silhouetteEdges;
      for(size_t j = 0; j < objects[i]->triangles.size(); j++){
        vec4 face_normal = glm::normalize(objects[i]->triangles[j].normal);
        if(glm::dot(face_normal,objects[i]->triangles[j].v0 - light->position ) >= 0){
          //Front facing to light
          vector<Edge> trianglesEdges;
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v0, objects[i]->triangles[j].v1));
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v1, objects[i]->triangles[j].v2));
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v2, objects[i]->triangles[j].v0));

          for(size_t k = 0; k < trianglesEdges.size(); k++){
            auto it = std::find_if(silhouetteEdges.begin(), silhouetteEdges.end(),
                      [&] (const Edge &otherEdge){
                        return otherEdge.CompareEdges(trianglesEdges[k]);
                      });
            if( it !=  silhouetteEdges.end()){
              silhouetteEdges.erase(it);
            }else{
              silhouetteEdges.push_back(trianglesEdges[k]);
            }
          }
        }

        */
      
  }

}
