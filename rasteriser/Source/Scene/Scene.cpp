#include "Scene.h"

using namespace std;
using glm::vec3;

Scene::Scene(){
    this->cam = new Camera();
    this->light = new Light();
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

//DRAW OBJECTS TO SCREEN
void Scene::Draw(screen* screen){
  //INITIALISE BUFFERS
  for (int i = 0 ; i < SCREEN_HEIGHT; i++)
  {
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
      cam->depthBuffer[i][j] = 0.0f;
      cam->stencilBuffer[i][j] = 0.0f;
      cam->stencilInvStore[i][j].clear();
    }
  }
  cam->initialisePixels();

  //CALL DRAWING METHODS
  if(!SHADOWS){
    for(size_t i = 0; i<objects.size(); i++){
      objects[i]->DrawPolygon(screen,cam,light);
    }
  }else{
    DrawPolygonsAmbient(screen);
    CreateShadowVolumes();
    DrawShadowVolumes(screen);
    DrawPolygonShadows(screen);
  }
  //POST_PROCESSING - AA TO SMOOTH EDGES
  post_processing(cam->pixels);
  //DRAW TO SDL SCREEN
  drawPixels(screen, cam);
}

//DRAW POLYGONS WITH AMBIENT LIGHTING
void Scene::DrawPolygonsAmbient(screen* screen){
  for(size_t i = 0; i<objects.size(); i++){
    objects[i]->DrawPolygonAmbient(screen,cam,light);
  }
}

//CREATE SHADOW VOLUMES FOR OBJECTS
void Scene::CreateShadowVolumes(){
  for(size_t i = 0; i<objects.size(); i++){
    //ALL OBJECTS EXCEPT CORNELL BOX
    if(i > 0){
      //INITIALISE STENCIL
      for (int i = 0 ; i < SCREEN_HEIGHT; i++){
        for (int j = 0; j < SCREEN_WIDTH; j++){
          cam->stencilWritten[i][j] = 0.0f;
        }
      }
      vector<Edge> silhouetteEdges;
      vector<Triangle> backfaces;
      //FIND BACK FACES AWAY FROM LIGHT USING DOT PRODUCT
      for(size_t j = 0; j < objects[i]->triangles.size(); j++){
        vec4 face_normal = glm::normalize(objects[i]->triangles[j].normal);
        vec4 averageTriPos = objects[i]->triangles[j].getAveragePosition();
        if(glm::dot(face_normal,averageTriPos - light->position ) >= 0){
          backfaces.push_back(objects[i]->triangles[j]);

          //CREATE TRIANGLE EDGES BELONGING TO BACK FACE FOUND
          vector<Edge> trianglesEdges;
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v0, objects[i]->triangles[j].v1));
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v1, objects[i]->triangles[j].v2));
          trianglesEdges.push_back(Edge(objects[i]->triangles[j].v2, objects[i]->triangles[j].v0));

          //ADD EDGE TO silhouetteEdges IF NOT ALREADY INSIDE OTHERWISE REMOVE - WE'RE FINDING ALL EDGES AT
          //BOUNDARY OF LIGHT AND DARK FACES
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
      //EXTRUSION MAGNITUDE - NORMALLY INFINITY
      float extrudeMagnitude = 2.0f;
      //MAKE QUADRILATERAL FROM EACH EDGE FOUND
      for(vector<Edge>::iterator it = silhouetteEdges.begin(); it != silhouetteEdges.end(); it++){
        shadowVolume.push_back(Triangle((*it).vertex1, (*it).vertex2, (*it).vertex1 + extrudeMagnitude * ((*it).vertex1 - light->position), vec3(1,1,1)));
        shadowVolume.push_back(Triangle((*it).vertex2, (*it).vertex1 + extrudeMagnitude * ((*it).vertex1 - light->position),
                                    (*it).vertex2 + extrudeMagnitude * ((*it).vertex2 - light->position), vec3(1,1,1)));
      }
      shadowVolume.insert(shadowVolume.end(), backfaces.begin(), backfaces.end());
      objects[i]->shadowVolume = shadowVolume;
    }
  }
}

//DRAW SHADOW VOLUMES
void Scene::DrawShadowVolumes(screen *screen){
  for(size_t i = 0; i<objects.size(); i++){
    //NOT CORNELL BOX
    if(i >0 && BUNNYSHADOWS){
      objects[i]->DrawShadowVolume(screen,cam,light);
    }
    else{
      //SHADOWS JUST FOR BOXES
      objects[1]->DrawShadowVolume(screen,cam,light);
      objects[2]->DrawShadowVolume(screen,cam,light);
    }
  }
}

//RENDER AGAIN WITH SHADOW VOLUMES INCLUDED
void Scene::DrawPolygonShadows(screen *screen){
  for(size_t i = 0; i<objects.size(); i++){
    //FIRST IS CORNELL BOX
    objects[i]->DrawPolygonShadow(screen,cam,light);
  }
}

//DRAW TO SDL SCREEN
void drawPixels(screen * screen, Camera* cam){
  for(int i = 0; i< SCREEN_WIDTH; i++){
    for(int j = 0; j < SCREEN_HEIGHT; j++){
      PutPixelSDL(screen,i,j,cam->pixels[i][j]);
    }
  }
}
