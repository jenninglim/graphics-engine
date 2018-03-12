#include <iostream>
#include <stdint.h>

#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include "Camera.h"
#include "Light.h"
//? TODO:why not imported in glm
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT SCREEN_WIDTH
#define CAM_FOCAL_LENGTH SCREEN_WIDTH
#define FULLSCREEN_MODE false
#define EPSILON 1e-2

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(screen* screen, vector<Triangle> &triangles,Camera cam, Light light);

void Interpolate(Pixel a, Pixel b, vector<Pixel>& result){
  int N = result.size();
  vec3 step = vec3(b.x - a.x, b.y - a.y, b.zinv - a.zinv) / float(glm::max(N-1, 1));
  vec3 illuminationStep = (b.illumination - a.illumination) / float(glm::max(N-1, 1));
  Pixel current = { a.x,
                   a.y,
                   a.zinv,
                   a.illumination
                  };

  for(int i =0; i <N; i++){
    result[i].x = current.x       + i*step.x;
    result[i].y = current.y       + i*step.y;
    result[i].zinv = current.zinv + i*step.z;
    result[i].illumination = current.illumination + float(i)*illuminationStep;
  }
}

void TransformationMatrix(mat4 &M, vec4 cameraPosition, mat4 cameraRotation)

{
  M = column(mat4(1), 3, cameraPosition) *
      column(cameraRotation,3,vec4(vec3(-1.0f *cameraPosition),1)) *
      column(mat4(1) ,3, vec4 (vec3(-1.0f * cameraPosition), 1));  
}


void VertexShader(const Vertex& v, Pixel& p, Camera cam, Light light)
{
  mat4 tMatrix(0);
  TransformationMatrix(tMatrix, cam.cameraPos,cam.R);
  vec4 tPosition = tMatrix * v.position;
  p.x = cam.focalLength*tPosition.x / tPosition.z + SCREEN_WIDTH / 2 ;
  p.y = cam.focalLength*tPosition.y / tPosition.z + SCREEN_HEIGHT /2;
  p.zinv = 1 / tPosition.z;
 
  vec4 r_hat = glm::normalize(light.position - v.position);
  float dist = glm::length(light.position - v.position);
  vec3 lightColour = light.power * glm::max(glm::dot(r_hat, v.normal), 0.0f) /
    (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));
  //Diffuse surface
  p.illumination =  (lightColour + light.indirect_light); 
}

void PixelShader(screen* screen, const Pixel& p, float (&depthBuffer)[SCREEN_HEIGHT][SCREEN_WIDTH], vec3 currentColor)
{
  int x = p.x;
  int y = p.y;
  if( p.zinv > depthBuffer[y][x] )
  {
    depthBuffer[y][x] = p.zinv;
    PutPixelSDL( screen, x, y, p.illumination* currentColor );
  }
}

void DrawLineSDL(screen* screen, Pixel a, Pixel b, vec3 color){
  ivec2 delta = abs(ivec2(a.x - b.x, a.y - b.y));
  int pixels = glm::max(delta.x, delta.y)+1;
  vector<Pixel> line(pixels);

  Interpolate(a,b,line);
  for(int pixel_num = 0; pixel_num < pixels; pixel_num++){
    PutPixelSDL(screen, line[pixel_num].x, line[pixel_num].y, color);
  } 
}
/*
void DrawPolygonEdges(screen* screen, const vector<Vertex>& vertices, Camera cam){
  int V = vertices.size(); 
  vector<Pixel> projectedVertices(V); 
  for(int i = 0; i < V; ++i){
    VertexShader(vertices[i], projectedVertices[i], cam);
  }
  for(int i=0; i < V; ++i)
  {
    int j = (i+1)%V; 
    vec3 color(1,1,1);
    DrawLineSDL(screen, projectedVertices[i], projectedVertices[j], color);
  }
}
*/
void ComputePolygonRows(const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels)
{
  int minY =glm::min(vertexPixels[0].y,glm::min(vertexPixels[1].y,vertexPixels[2].y));
  int maxY =glm::max(vertexPixels[0].y,glm::max(vertexPixels[1].y,vertexPixels[2].y));
  int rowNum = maxY - minY + 1;
  
  leftPixels.resize(rowNum);
  rightPixels.resize(rowNum);
  
  for(int i = 0; i < rowNum; i++){
    leftPixels[i].x = numeric_limits<int>::max();
    rightPixels[i].x = numeric_limits<int>::min(); 
  }

  
  for(int i = 0; i < 3; i++){
    Pixel vertex1 = vertexPixels[i];
    Pixel vertex2 = vertexPixels[(i+1)%3];
    //TODO : same as in drawlinesdl - clean
    ivec2 delta = abs(ivec2(vertex1.x - vertex2.x, vertex1.y - vertex2.y));
    int pixels = glm::max(delta.x, delta.y)+1;
    vector<Pixel> line(pixels);
    Interpolate(vertex1, vertex2, line);
     
    for(int pixel_num = 0; pixel_num < pixels; pixel_num++){
      if(line[pixel_num].y <= maxY &&  line[pixel_num].y >= minY){
        if(leftPixels[line[pixel_num].y -minY].x > line[pixel_num].x){
        //Replace
          leftPixels[line[pixel_num].y - minY] = line[pixel_num]; 
        }
        if(rightPixels[line[pixel_num].y - minY].x < line[pixel_num].x){
        //Replace
          rightPixels[line[pixel_num].y-minY] = line[pixel_num];
        }
      }
    }
  }
}

void DrawPolygonRows(screen* screen,
        const vector<Pixel>& leftPixels,
        const vector<Pixel>& rightPixels,
        vec3 color,
        float (&depthBuffer)[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    for(unsigned int row = 0; row < leftPixels.size(); row++)
    {
            /*
          Pixel p = {.x = pixelX, .y = leftPixels[row].y,
              .zinv = leftPixels[row].zinv,
              .illumination = leftPixels[row].illumination};  
              */
          vector<Pixel> pixxes(rightPixels[row].x - leftPixels[row].x + 1);
          Interpolate(leftPixels[row], rightPixels[row], pixxes);
          for (int i = 0; i < pixxes.size(); i++)
          {
            PixelShader(screen,pixxes[i], depthBuffer,color);  
          }
     }
}

void DrawPolygon(screen* screen,
        const vector<Vertex>& vertices,
        vec3 color,
        Camera cam,
        float (&depthBuffer)[SCREEN_HEIGHT][SCREEN_WIDTH],
        Light light)
{
  int V = vertices.size();
  vector<Pixel> vertexPixels(V); 
  for(int i = 0; i<V; ++i){
    VertexShader(vertices[i], vertexPixels[i], cam, light); 
  }
  vector<Pixel> leftPixels;
  vector<Pixel> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  DrawPolygonRows(screen,leftPixels, rightPixels,color, depthBuffer);
}


int main( int argc, char* argv[] )
{
  
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

  //get triangles
  vector<Triangle> triangles;
  LoadTestModel(triangles);  

  //create camera
  vec4 camPos(0,0,-3.001,1);
  Camera cam(CAM_FOCAL_LENGTH, camPos); 

  //create light
  Light light(vec4(0,-0.5,-0.7, 1), 100.1f*vec3(1,1,1));
  while( NoQuitMessageSDL() )
    {
      Update();
      Draw(screen,triangles,cam,light);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, vector<Triangle> &triangles, Camera cam, Light light)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));
  float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
  for (int i = 0 ; i < SCREEN_HEIGHT; i++)
  {
      for (int j = 0; j < SCREEN_WIDTH; j++)
      {
          depthBuffer[i][j] = 0.0f;
      }
  }

  for(uint32_t i=0; i<triangles.size(); ++i){
    vector<Vertex> vertices(3);
    vertices[0].position = triangles[i].v0;
    vertices[0].normal = glm::normalize(triangles[i].normal);
    vertices[0].reflectance = vec2 (1,1);
    vertices[1].position = triangles[i].v1; 
    vertices[1].normal = glm::normalize(triangles[i].normal);
    vertices[1].reflectance = vec2 (1,1);
                  
    vertices[2].position = triangles[i].v2; 
    vertices[2].normal = glm::normalize(triangles[i].normal);
    vertices[2].reflectance = vec2 (1,1);
    DrawPolygon(screen, vertices, triangles[i].color, cam, depthBuffer, light);
    //DrawPolygonEdges(screen, vertices,cam);
    /*
    for(int v=0; v<3; ++v){
      ivec2 projPos;
      VertexShader(vertices[v], projPos,cam); 
      
      std::cout << glm::to_string(projPos) << std::endl;
      vec3 color(1,1,1);
      PutPixelSDL(screen, projPos.x, projPos.y, color);
    }
    */
  }  

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
