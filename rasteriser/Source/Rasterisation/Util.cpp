#include "Util.h"
#include <algorithm>
using namespace std;
using namespace glm;


void Interpolate(Pixel a, Pixel b, vector<Pixel>& result){

  int N = result.size();
  vec3 step = vec3(b.x - a.x, b.y - a.y, b.zinv - a.zinv) / float(glm::max(N-1, 1));
  vec4 position3dstep  = (b.pos3d - a.pos3d) / float(glm::max(N-1, 1));
  Pixel current = {a.x,
                   a.y,
                   a.zinv,
                   a.pos3d
                  };

  for(int i =0; i <N; i++){
    result[i].x = current.x       + i*step.x;
    result[i].y = current.y       + i*step.y;
    result[i].zinv = current.zinv + i*step.z;
    result[i].pos3d = current.pos3d + float(i)*position3dstep;
  }
}

void TransformationMatrix(mat4 &M, vec4 cameraPosition, mat4 cameraRotation)

{
  M = column(mat4(1), 3, cameraPosition) *
      column(cameraRotation,3,vec4(vec3(-1.0f *cameraPosition),1)) *
      column(mat4(1) ,3, vec4 (vec3(-1.0f * cameraPosition), 1));
}

void VertexShader(const Vertex& v, Pixel& p, Camera* cam, Light* light)
{
  mat4 tMatrix(0);
  TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
  vec4 tPosition = tMatrix * v.position;
  p.x = cam->focalLength*tPosition.x / tPosition.z + SCREEN_WIDTH / 2 ;
  p.y = cam->focalLength*tPosition.y / tPosition.z + SCREEN_HEIGHT /2;
  p.zinv = 1 / tPosition.z;
  p.pos3d = v.position * p.zinv;
}

void PixelShader(screen* screen,
                 const Pixel& p,
                 vec3 currentColor,
                 Camera *cam,
                 Light* light,
                 vec4 currentNormal,
                 vec3 currentReflectance,
                 Draw type)
{

  if(type == Draw::SCENE_AMBIENT){
    vec3 finalColour = currentReflectance * (/*lightColour +*/ light->indirect_light);
    int x = p.x ;
    int y = p.y ;
    if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
      if( p.zinv > cam->depthBuffer[y][x])
      {
        cam->depthBuffer[y][x] = p.zinv;
        cam->pixelPoint3d[y][x] = p.pos3d;
        PutPixelSDL( screen, x, y, finalColour* currentColor );
      }
    }
  }

  if(type == Draw::SCENE_SHADOW){
    int x = p.x ;
    int y = p.y ;
    if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
      if(cam->stencilBuffer[y][x] < 0){
        cout << cam->stencilBuffer[y][x] << endl;
      }
      if( p.zinv == cam->depthBuffer[y][x] && cam->stencilBuffer[y][x] <= 0)
      {
        //cout << " stencil buffer 0" << endl;
        vec4 r_hat = glm::normalize(light->position - (p.pos3d/p.zinv));
        float dist = glm::length(light->position - (p.pos3d/p.zinv));
        vec3 lightColour = light->power * glm::max(glm::dot(r_hat, currentNormal), 0.0f) /
          (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));
        vec3 finalColour = currentReflectance * (lightColour + light->indirect_light);
        PutPixelSDL( screen, x, y, finalColour* currentColor );
      }
    }
  }

  else if(type == Draw::SHADOW){
    int x = p.x;
    int y = p.y;
    if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
      if(p.zinv > cam->depthBuffer[y][x]){
        auto it = std::find_if(cam->stencilInvStore[y][x].begin(), cam->stencilInvStore[y][x].end(),
                    [=] (float value){
                      return p.zinv == value;
                    });
        if(it == cam->stencilInvStore[y][x].end() ){

          //Not contained in stencil inv store
          if(cam->stencilWritten[y][x] == 1){
              //HAS BEEN WRITTEN FOR THIS OBJECT
            cam->stencilBuffer[y][x] = (cam->stencilBuffer[y][x] - 1.0f);
              //cout << "stencil minus" << endl;
            cam->stencilInvStore[y][x].push_back(p.zinv);
          }else{
            cam->stencilBuffer[y][x] = (cam->stencilBuffer[y][x] + 1.0f);
              //cout << "stencil plus" << endl;
            cam->stencilWritten[y][x] = 1;
            cam->stencilInvStore[y][x].push_back(p.zinv);
          }
        }else{
          cout << "Duplicate found" << endl;
        }
      }
    }
  }
}

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
        Camera* cam,
        Light* light,
        vec4 currentNormal,
        vec3 currentReflectance,
        Draw type)
{

    for(unsigned int row = 0; row < leftPixels.size(); row++)
    {
      vector<Pixel> pixxes(rightPixels[row].x - leftPixels[row].x + 1);
      Interpolate(leftPixels[row], rightPixels[row], pixxes);
      for (size_t i = 0; i < pixxes.size(); i++)
      {
        PixelShader(screen,pixxes[i],color,cam, light, currentNormal,currentReflectance, type);
      }
    }
}

void DrawPolygonRasterisation(screen* screen,
        const vector<Vertex>& vertices,
        vec3 color,
        Camera* cam,
        Light* light,
        vec4 currentNormal,
        vec3 currentReflectance,
        Draw type)
{

  int V = vertices.size();
  vector<Pixel> vertexPixels(V);

  for(int i = 0; i<V; ++i){
    VertexShader(vertices[i], vertexPixels[i], cam, light);
  }
  vector<Pixel> leftPixels;
  vector<Pixel> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  DrawPolygonRows(screen,leftPixels, rightPixels, color, cam, light, currentNormal, currentReflectance, type);
}
