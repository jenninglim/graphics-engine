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

void setProjectionMatrix(mat4 &P){
  float angleofview = (float)ANGLEOFVIEW;
  float nearplane = (float)NEARPLANE;
  float farplane = (float)FARPLANE;
  float scale = 1 / glm::tan(angleofview * 0.5 * glm::pi<float>() / 180);
  P[0][0] = scale;
  P[1][1] = scale;
  P[2][2] = farplane / (farplane - nearplane);
  P[3][2] = (-1 *farplane * nearplane) / (farplane - nearplane);
  P[2][3] = 1;
  P[3][3] = 0;

}

void multiPointMatrix(vec4 &result, vec4 &v_in, mat4 &m_in){
  result = v_in * glm::transpose(m_in);
  //Transpose because of glm autoconversion
  for(int testi = 0; testi < 4; testi++){
    for(int testj = 0; testj < 4; testj++){
    }
  }

}

void ClipOnWAxis(vector<Pixel> &conicalPixels, Camera *cam){


  vector<Pixel> newConicalPixels;
  for(int i = 0; i < conicalPixels.size(); i++ ){
    int current = i;
    float proportion;
    int prev = (i - 1 + conicalPixels.size()) % conicalPixels.size();
    int prevSign = conicalPixels[prev].conicalPos.w < W_CLIPPING_PLANE ? -1 : 1;
    int currentSign = conicalPixels[current].conicalPos.w < W_CLIPPING_PLANE ? -1 : 1;
    if((prevSign * currentSign) < 0){
      //The edge goes over w = 0 plane
      vec4 newIntersectionPoint;
      proportion = abs((W_CLIPPING_PLANE - conicalPixels[prev].conicalPos.w)) /
                    (abs(conicalPixels[prev].conicalPos.w) + abs(conicalPixels[current].conicalPos.w));
      newIntersectionPoint = conicalPixels[prev].conicalPos + proportion *
                                (conicalPixels[current].conicalPos - conicalPixels[prev].conicalPos);
      Pixel newPixel;
      newPixel.conicalPos = newIntersectionPoint;

      //Finding Zinv + Pos3d
      mat4 tMatrix(0), tMatrixInv(0);
      TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
      tMatrixInv = glm::inverse(tMatrix);

      mat4 projMatrix(0), projMatrixInv(0);
      setProjectionMatrix(projMatrix);
      projMatrixInv = glm::inverse(projMatrix);
      vec4 tPosition;
      multiPointMatrix(tPosition,newPixel.conicalPos,projMatrixInv);
      newPixel.zinv = 1 / tPosition.z;
      newPixel.pos3d = (tMatrixInv * tPosition) * newPixel.zinv;
      newConicalPixels.push_back(newPixel);
    }

    if(currentSign > 0){
      newConicalPixels.push_back(conicalPixels[current]);
    }
  }
  conicalPixels = newConicalPixels;

}

void VertexShader(const Vertex& v, Pixel& p, Camera* cam, Light* light)
{
  mat4 tMatrix(0);
  TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
  vec4 tPosition = tMatrix * v.position;


  mat4 projMatrix(0);
  setProjectionMatrix(projMatrix);

  multiPointMatrix(p.conicalPos, tPosition, projMatrix);


  p.zinv = 1 / tPosition.z;
  p.pos3d = v.position * p.zinv;


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
      cam->pixels[x][y] = finalColour* currentColor;
      //PutPixelSDL( screen, x, y, finalColour* currentColor );
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

void ComputeLinePlaneIntersection(Pixel &newIntersectionVertex, Pixel &S,  Pixel&E, vec4 clippingPlanePoint, vec4 clippingPlaneNormal){

  vec3 l = glm::normalize(vec3(S.conicalPos) - vec3(E.conicalPos));
  float d = (float)glm::dot((vec3(clippingPlanePoint) - vec3(E.conicalPos)),vec3(clippingPlaneNormal)) / (float)(glm::dot(l, vec3(clippingPlaneNormal)));

  vec3 intersection = d * l + vec3(E.conicalPos);
  float proportionToIntersection = (float)glm::length(d * l) / (float)glm::length(vec3(S.conicalPos) - vec3(E.conicalPos));

  newIntersectionVertex.zinv = (float)(S.zinv - E.zinv) * proportionToIntersection + E.zinv;

  newIntersectionVertex.pos3d = (S.pos3d - E.pos3d) * proportionToIntersection + E.pos3d;
  newIntersectionVertex.conicalPos = vec4(intersection,1);
}


void Sutherland_Hodgman(vector<Pixel> &outputVertex){
  vector<vec4> clippingPlanes = {vec4(1,0,0,0), vec4(-1,0,0,0),vec4(0,1,0,0), vec4(0,-1,0,0), vec4(0,0,1,0), vec4(0,0,0,0)};
  vector<vec4> clippingNormals ={vec4(-1,0,0,0), vec4(1,0,0,0),vec4(0,-1,0,0), vec4(0,1,0,0), vec4(0,0,-1,0), vec4(0,0,1,0)};
  for(size_t planeindex = 0; planeindex < clippingPlanes.size(); planeindex++){

    vector<Pixel> inputList = outputVertex;
    outputVertex.clear();
    Pixel S;
    if(inputList.size() > 0){
      S = inputList.back();
    }else{
      return;
    }
    for(size_t vertexindex = 0; vertexindex < inputList.size(); vertexindex++){
      Pixel E = inputList[vertexindex];
      vec4 planenormal = clippingNormals[planeindex];
      if(glm::dot(planenormal, E.conicalPos - clippingPlanes[planeindex]) >= 0){
        //E Inside plane
          if(glm::dot(planenormal, S.conicalPos - clippingPlanes[planeindex]) < 0){
          //S Outside plane
          Pixel newIntersectionVertex;
          ComputeLinePlaneIntersection(newIntersectionVertex, S,E,clippingPlanes[planeindex], planenormal);

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
      }else if(glm::dot(planenormal,S.conicalPos - clippingPlanes[planeindex]) >= 0){
        Pixel newIntersectionVertex;
        ComputeLinePlaneIntersection(newIntersectionVertex, E,S,clippingPlanes[planeindex], planenormal);
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

void WDivision(vector<Pixel>&conicalPixels){
  for(int i = 0; i < conicalPixels.size(); i++){
    if(conicalPixels[i].conicalPos.w != 1){
      conicalPixels[i].conicalPos.x /= conicalPixels[i].conicalPos.w;
      conicalPixels[i].conicalPos.y /= conicalPixels[i].conicalPos.w;
      conicalPixels[i].conicalPos.z /= conicalPixels[i].conicalPos.w;
      conicalPixels[i].conicalPos.w = 1;
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



  int V = vertices.size();
  vector<Pixel> conicalPixel(V);
  vector<Pixel> vertexPixels(V);
  for(int i = 0; i<V; ++i){
    VertexShader(vertices[i], conicalPixel[i], cam, light);
  }


  //Sort the w in the triangle
  ClipOnWAxis(conicalPixel,cam);

  WDivision(conicalPixel);
  Sutherland_Hodgman(conicalPixel);

  mat4 tMatrix(0), tMatrixInv(0);
  TransformationMatrix(tMatrix, cam->cameraPos,cam->R);
  tMatrixInv = glm::inverse(tMatrix);

  mat4 projMatrix(0), projMatrixInv(0);
  setProjectionMatrix(projMatrix);
  projMatrixInv = glm::inverse(projMatrix);


  for(size_t pixel = 0; pixel < conicalPixel.size(); pixel++){
    conicalPixel[pixel].x = glm::min(SCREEN_WIDTH-1,(int)(((conicalPixel[pixel].conicalPos.x+1) * 0.5) *SCREEN_WIDTH));
    conicalPixel[pixel].y = glm::min(SCREEN_HEIGHT-1, (int)(((conicalPixel[pixel].conicalPos.y+1)*0.5)*SCREEN_HEIGHT));
  }


  if(conicalPixel.size() > 0){
    for(int y = 1; y < conicalPixel.size() - 1; y++ ){

      vertexPixels[0] = conicalPixel[0];
      vertexPixels[1] = conicalPixel[y];
      vertexPixels[2] = conicalPixel[y+1];
      vector<Pixel> leftPixels;
      vector<Pixel> rightPixels;
      for(size_t testvertex = 0; testvertex < vertexPixels.size(); testvertex++){

      }
      ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
      for(size_t testvertex = 0; testvertex < leftPixels.size(); testvertex++){

      }
      DrawPolygonRows(screen,leftPixels, rightPixels, color, cam, light, currentNormal, currentReflectance);
    }
  }
}
