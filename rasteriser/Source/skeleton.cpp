#include <iostream>
#include <stdint.h>

#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include "Camera.h"
//? TODO:why not imported in glm
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define CAM_FOCAL_LENGTH SCREEN_WIDTH
#define FULLSCREEN_MODE false

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(screen* screen, vector<Triangle> &triangles,Camera cam);

void Interpolate(ivec2 a, ivec2 b, vector<ivec2>& result){
  int N = result.size();
  vec2 step = vec2(b-a) / float(glm::max(N-1, 1)); 
  vec2 current(a);
  for(int i =0; i <N; ++i){
    result[i] = current; 
    current += step; 
  }
}


void TransformationMatrix(mat4 &M, vec4 cameraPosition, mat4 cameraRotation)
{

  M = column(mat4(1), 3, cameraPosition) * column(cameraRotation,3,vec4(vec3(-1.0f *cameraPosition),1)) *  column(mat4(1) ,3, vec4 (vec3(-1.0f * cameraPosition), 1));  
}


void VertexShader(const vec4& v, ivec2& p, Camera cam)
{
  mat4 tMatrix(0);
  TransformationMatrix(tMatrix,cam.cameraPos,cam.R);
  vec4 tPosition = tMatrix * v;
  p.x = cam.focalLength*tPosition.x / tPosition.z + SCREEN_WIDTH / 2 ;
  p.y = cam.focalLength*tPosition.y / tPosition.z + SCREEN_HEIGHT /2;
  
}

void DrawLineSDL(screen* screen, ivec2 a, ivec2 b, vec3 color){
  ivec2 delta = abs(a-b); 
  int pixels = glm::max(delta.x, delta.y)+1;
  vector<ivec2> line(pixels);
  Interpolate(a,b,line);
  for(int pixel_num = 0; pixel_num < pixels; pixel_num++){
    PutPixelSDL(screen, line[pixel_num].x, line[pixel_num].y, color);

  } 
}

void DrawPolygonEdges(screen* screen, const vector<vec4>& vertices, Camera cam){
  int V = vertices.size(); 
  vector<ivec2> projectedVertices(V); 
  for(int i = 0; i < V; ++i){
    VertexShader(vertices[i], projectedVertices[i], cam);
  }
  for(int i=0; i < V; ++i){
    int j = (i+1)%V; 
    vec3 color(1,1,1);
    DrawLineSDL(screen, projectedVertices[i], projectedVertices[j], color);
  }

}

void ComputePolygonRows(const vector<ivec2>& vertexPixels, vector<ivec2>& leftPixels, vector<ivec2>& rightPixels){
  int minY =glm::min(vertexPixels[0].y,glm::min(vertexPixels[1].y,vertexPixels[2].y));
  int maxY =glm::max(vertexPixels[0].y,glm::max(vertexPixels[1].y,vertexPixels[2].y));
  int rowNum = maxY - minY + 1;
  
  leftPixels.resize(rowNum);
  rightPixels.resize(rowNum);
  
  for(int i = 0; i < rowNum; i++){
    leftPixels[i].x = +numeric_limits<int>::max();
    rightPixels[i].x = -numeric_limits<int>::max(); 
  }

  for(int i = 0; i < 3; i++){
    ivec2 vertex1 = vertexPixels[i];
    ivec2 vertex2 = vertexPixels[(i+1)%3];
    //TODO : same as in drawlinesdl - clean
    ivec2 delta = abs(vertex1- vertex2);
    int pixels = glm::max(delta.x, delta.y)+1;
    vector<ivec2> line(pixels);
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

void DrawPolygonRows(screen* screen, const vector<ivec2>& leftPixels, const vector<ivec2>& rightPixels,vec3 color){
  for(unsigned int row = 0; row < leftPixels.size(); row++){
    for(int pixelX = leftPixels[row].x; pixelX <= rightPixels[row].x; pixelX++){
      PutPixelSDL(screen, pixelX,leftPixels[row].y, color);
    } 
  }
}

void DrawPolygon(screen* screen, const vector<vec4>& vertices,vec3 color, Camera cam){
  int V = vertices.size();
  vector<ivec2> vertexPixels(V); 
  for(int i = 0; i<V; ++i){
    VertexShader(vertices[i], vertexPixels[i], cam); 
  }
  vector<ivec2> leftPixels;
  vector<ivec2> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  DrawPolygonRows(screen,leftPixels, rightPixels,color);
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

  while( NoQuitMessageSDL() )
    {
      Update();
      Draw(screen,triangles,cam);
      SDL_Renderframe(screen);
    }

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void Draw(screen* screen, vector<Triangle> &triangles, Camera cam)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

  for(uint32_t i=0; i<triangles.size(); ++i){
    vector<vec4> vertices(3);
    vertices[0] = triangles[i].v0; 
    vertices[1] = triangles[i].v1; 
    vertices[2] = triangles[i].v2; 
    DrawPolygon(screen, vertices, triangles[i].color, cam);
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
