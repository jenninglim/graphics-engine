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

void setProjectionMatrix(mat4 &P, float nearPlane, float  farPlane, float angleOfView){
  float scale = 1 / glm::tan(angleOfView * 0.5 * glm::pi<float>() / 180);
  P[0][0] = scale;
  P[1][1] = scale;
  P[2][2] = -1*farPlane / (farPlane - nearPlane);
  P[3][2] = (-1 *farPlane * nearPlane) / (farPlane - nearPlane);
  P[2][3] = 1;
  P[3][3] = 0;
}

void multiPointMatrix(vec4 &result, vec4 &v_in, mat4 &m_in){
  result = v_in * m_in;
}

void VertexShader(const Vertex& v, Pixel& p, Camera* cam, Light* light)
{
  mat4 tMatrix(0);
  TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
  vec4 tPosition = tMatrix * v.position;

  float nearPlane = 1;
  float farPlane = 20;
  float angleOfView = 90;

  mat4 projMatrix(0);
  setProjectionMatrix(projMatrix, nearPlane, farPlane, angleOfView);
  multiPointMatrix(p.conicalPos, tPosition, projMatrix);
  p.w = p.conicalPos.w;

  if(p.conicalPos.w != 1){
    p.conicalPos.x /= p.conicalPos.w;
    p.conicalPos.y /= p.conicalPos.w;
    p.conicalPos.z /= p.conicalPos.w;
    p.conicalPos.w = 1;
  }


  //p.x = cam->focalLength*tPosition.x / tPosition.z + SCREEN_WIDTH / 2 ;
  //p.y = cam->focalLength*tPosition.y / tPosition.z + SCREEN_HEIGHT /2;
  //p.x = glm::min(SCREEN_WIDTH-1,(int)((1-(conicalVertex.x+1) * 0.5) *SCREEN_WIDTH));
  //p.y = glm::min(SCREEN_HEIGHT-1, (int)((1-(conicalVertex.y+1)*0.5)*SCREEN_HEIGHT));
  //p.zinv = 1 / tPosition.z;
  //p.pos3d = v.position * p.zinv;
}

void PixelShader(screen* screen,
                 const Pixel& p,
                 vec3 currentColor,
                 Camera *cam,
                 Light* light,
                 vec4 currentNormal,
                 vec3 currentReflectance)
{
  vec4 l_hat = glm::normalize(light->position - (p.pos3d/p.zinv));
  float dist = glm::length(light->position - (p.pos3d/p.zinv));
  vec3 diffuseColor = light->diffuseLightIntensity * glm::max(glm::dot(l_hat, currentNormal), 0.0f) /
    (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));

  vec4 r_hat = glm::normalize((2 * glm::max(glm::dot(l_hat, currentNormal), 0.0f) * currentNormal) - l_hat);
  vec4 v_hat = glm::normalize(cam->cameraPos - (p.pos3d/p.zinv));
  vec3 specularColor = light->specularLightIntensity * glm::pow(glm::max(glm::dot(r_hat, v_hat), 0.0f), 3.0f) /
    (float) (4.0f * glm::pi<float>() * glm::pow<float>(dist,2));;

  //Diffuse surface

  vec3 finalColour = currentReflectance * ((diffuseColor != vec3(0) ? specularColor + diffuseColor : vec3(0)) + light->ambientLightIntensity);

  int x = p.x ;
  int y = p.y ;
  if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
    if( p.zinv > cam->depthBuffer[y][x] )
    {
      cam->depthBuffer[y][x] = p.zinv;
      PutPixelSDL( screen, x, y, finalColour* currentColor );
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
        vec3 currentReflectance)
{

    for(unsigned int row = 0; row < leftPixels.size(); row++)
    {
      vector<Pixel> pixxes(rightPixels[row].x - leftPixels[row].x + 1);
      Interpolate(leftPixels[row], rightPixels[row], pixxes);
      for (size_t i = 0; i < pixxes.size(); i++)
      {
        PixelShader(screen,pixxes[i],color,cam, light, currentNormal,currentReflectance);
      }
    }
}

vec4 ComputeLinePlaneIntersection( Pixel &S,  Pixel&E, vec4 clippingPlanePoint, vec4 clippingPlaneNormal){
  vec3 l = (vec3(S.conicalPos) - vec3(E.conicalPos));
  float d = glm::dot((vec3(clippingPlanePoint) - vec3(E.conicalPos)),vec3(clippingPlaneNormal)) / (glm::dot(l, vec3(clippingPlaneNormal)));
  return vec4((d * l + vec3(E.conicalPos)),1);
}

void Sutherland_Hodgman(vector<Pixel> &outputVertex){
  vector<vec4> clippingPlanes = {vec4(1,0,0,1), vec4(-1,0,0,1),vec4(0,1,0,1), vec4(0,-1,0,1), vec4(0,0,1,1), vec4(0,0,0,1)};
  vector<vec4> clippingNormals ={vec4(-1,0,0,1), vec4(1,0,0,1),vec4(0,-1,0,1), vec4(0,1,0,1), vec4(0,0,-1,1), vec4(0,0,1,1)};
  for(size_t planeindex = 0; planeindex < clippingPlanes.size(); planeindex++){
    vector<Pixel> inputList = outputVertex;
    outputVertex.clear();
    Pixel S = inputList.back();
    for(size_t vertexindex = 0; vertexindex < inputList.size(); vertexindex++){
      Pixel E = inputList[vertexindex];
      vec4 planenormal = clippingNormals[planeindex];
      if(glm::dot(planenormal, E.conicalPos) >= 0){
        //E Inside plane
        if(glm::dot(planenormal, S.conicalPos) < 0){
          //S Outside plane
          Pixel newIntersectionVertex;
          newIntersectionVertex.conicalPos = ComputeLinePlaneIntersection(S,E,clippingPlanes[planeindex], planenormal);
          auto it  = std::find_if(outputVertex.begin(), outputVertex.end(),
            [&](Pixel &pixel) {return pixel.conicalPos == newIntersectionVertex.conicalPos;});
          if(it == outputVertex.end()){
            outputVertex.push_back(newIntersectionVertex);
          }
        }
        auto it  = std::find_if(outputVertex.begin(), outputVertex.end(),
          [&](Pixel &pixel) {return pixel.conicalPos == E.conicalPos;});
        if(it == outputVertex.end()){
          outputVertex.push_back(E);
        }
      }else if(glm::dot(planenormal,S.conicalPos) >= 0){
        Pixel newIntersectionVertex;
        newIntersectionVertex.conicalPos = ComputeLinePlaneIntersection(S,E,clippingPlanes[planeindex], planenormal);
        auto it  = std::find_if(outputVertex.begin(), outputVertex.end(),
          [&](Pixel &pixel) {return pixel.conicalPos == newIntersectionVertex.conicalPos;});
        if(it == outputVertex.end()){
          outputVertex.push_back(newIntersectionVertex);
        }
      }
      S = E;
    }
  }
}

void DrawPolygonRasterisation(screen* screen,
        const vector<Vertex>& vertices,
        vec3 color,
        Camera* cam,
        Light* light,
        vec4 currentNormal,
        vec3 currentReflectance)
{
  for(size_t vindex = 0; vindex < vertices.size(); vindex++){
    //cout << glm::to_string(vertices[vindex].position) << endl;
  }
  //Beforevec4(1.053813, 1.202116, -0.202116, 1.000000)
  //Beforevec4(-1.030182, 0.928311, 0.071689, 1.000000)
  //Beforevec4(0.379680, 0.342140, 0.657860, 1.000000)

  Pixel p0; p0.conicalPos = vec4(1.053813, 1.202116, -0.202116, 1.000000);
  Pixel p1; p1.conicalPos = vec4(-1.030182, 0.928311, 0.071689, 1.000000);
  Pixel p2; p2.conicalPos = vec4(0.379680, 0.342140, 0.657860, 1.000000);
  int V = vertices.size();
  vector<Pixel> conicalPixel({p0,p1,p2});
  vector<Pixel> vertexPixels(V);
  //for(int i = 0; i<V; ++i){
  //  VertexShader(vertices[i], conicalPixel[i], cam, light);
  //}
  //conicalPixel.push_back(p0);
  //conicalPixel.push_back(p1);
  //conicalPixel.push_back(p2);
  for(size_t vindex = 0; vindex < conicalPixel.size(); vindex++){
    cout << "Before" << glm::to_string(conicalPixel[vindex].conicalPos) << endl;
  }
  Sutherland_Hodgman(conicalPixel);
  cout << "Round" << endl;
  for(size_t vindex = 0; vindex < conicalPixel.size(); vindex++){
    cout << glm::to_string(conicalPixel[vindex].conicalPos) << endl;
  }
  mat4 tMatrix(0), tMatrixInv(0);
  TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
  tMatrixInv = glm::inverse(tMatrix);
  float nearPlane = 1;
  float farPlane = 20;
  float angleOfView = 90;
  mat4 projMatrix(0), projMatrixInv(0);
  setProjectionMatrix(projMatrix, nearPlane, farPlane, angleOfView);
  projMatrixInv = glm::inverse(projMatrix);

  for(size_t pixel; pixel < conicalPixel.size(); pixel++){
    vec4 tPosition = conicalPixel[pixel].conicalPos * conicalPixel[pixel].w * projMatrixInv;
    conicalPixel[pixel].zinv = 1 / tPosition.z;
    vec4 v = tMatrixInv * tPosition ;
    conicalPixel[pixel].pos3d = v * conicalPixel[pixel].zinv;
    conicalPixel[pixel].x = glm::min(SCREEN_WIDTH-1,(int)((1-(conicalPixel[pixel].conicalPos.x+1) * 0.5) *SCREEN_WIDTH));
    conicalPixel[pixel].y = glm::min(SCREEN_HEIGHT-1, (int)((1-(conicalPixel[pixel].conicalPos.y+1)*0.5)*SCREEN_HEIGHT));
  }
  for(size_t vindex = 0; vindex < vertices.size(); vindex++){
    //cout << glm::to_string(conicalPixel[vindex].pos3d / conicalPixel[vindex].zinv) << endl;
  }

  //cout << conicalPixel.size() << endl;
  if(conicalPixel.size() == 4){
    for(int y = 0; y < 2; y++ ){
      for(int j = 0; j < vertexPixels.size(); j++){
        vertexPixels[j] = conicalPixel[j+y];
        vector<Pixel> leftPixels;
        vector<Pixel> rightPixels;
        ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
        DrawPolygonRows(screen,leftPixels, rightPixels, color, cam, light, currentNormal, currentReflectance);
      }
    }
  }


  if(conicalPixel.size() == 3){
    vertexPixels = conicalPixel;
    vector<Pixel> leftPixels;
    vector<Pixel> rightPixels;
    ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
    DrawPolygonRows(screen,leftPixels, rightPixels, color, cam, light, currentNormal, currentReflectance);
  }
}
