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
      cam->stencilBuffer[i][j] = 0.0f;
      cam->pixelPoint3d[i][j] = vec4(0);
      cam->stencilInvStore[i][j].clear();
    }
  }
  //DRAW THE POLYGONS
  for(size_t i = 0; i<objects.size(); i++){
    //FIRST IS CORNELL BOX
    objects[i]->DrawPolygonAmbient(screen,cam,light);
  }

  for(size_t i = 0; i<objects.size(); i++){
    //OBJECT IN SCENE
    if(i > 0){
      for (int i = 0 ; i < SCREEN_HEIGHT; i++)
      {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
          cam->stencilWritten[i][j] = 0.0f;
        }
      }
      vector<Edge> silhouetteEdges;
      vector<Triangle> backfaces;
      for(size_t j = 0; j < objects[i]->triangles.size(); j++){
        vec4 face_normal = glm::normalize(objects[i]->triangles[j].normal);
        vec4 averageTriPos = objects[i]->triangles[j].getAveragePosition();
        if(glm::dot(face_normal,averageTriPos - light->position ) >= 0){
          backfaces.push_back(objects[i]->triangles[j]);

          //Back facing to light
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
      }


      vector<Triangle> shadowVolume;

      float extrudeMagnitude = 2.0f;

      for(vector<Edge>::iterator it = silhouetteEdges.begin(); it != silhouetteEdges.end(); it++){
        shadowVolume.push_back(Triangle((*it).vertex1, (*it).vertex2, (*it).vertex1 + extrudeMagnitude * ((*it).vertex1 - light->position), vec3(1,1,1)));
        shadowVolume.push_back(Triangle((*it).vertex2, (*it).vertex1 + extrudeMagnitude * ((*it).vertex1 - light->position),
                                        (*it).vertex2 + extrudeMagnitude * ((*it).vertex2 - light->position), vec3(1,1,1)));
      }
      shadowVolume.insert(shadowVolume.end(), backfaces.begin(), backfaces.end());
      objects[i]->shadowVolume = shadowVolume;
      objects[i]->DrawShadowVolume(screen,cam,light);
    }
  }
  //cout << "Print stencil Buffer" << endl;
  //cout << cam->stencilBuffer[100][100] << endl;
  //DRAW THE POLYGONS

  for(size_t i = 0; i<objects.size(); i++){
    //FIRST IS CORNELL BOX
    objects[i]->DrawPolygonShadow(screen,cam,light);
  }






}
