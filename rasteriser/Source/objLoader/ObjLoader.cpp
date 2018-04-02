#include "ObjLoader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <string.h>

using namespace std;
using glm::vec4;
using glm::vec3;

bool loadObj(
        const char * path,
        vector<vec4> & out_vertices,
        vector<vec3> & out_faces)
{
    bool load = false;
    FILE * file = fopen(path, "r");
    vec3 v, f;


    vector<vec4> temp_vertices;
    vector<vec3> temp_faces;
    if (path != NULL)
    {
      if(file != NULL){
        cout << "Path found" << endl;
      }
        while (1)
        {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF){
                cout << "Done" << endl;
                break;
            }
            if (strcmp(lineHeader, "v") == 0)
            {
                res = fscanf(file,
                        "%f %f %f\n",
                        &v[0],
                        &v[1],
                        &v[2]);
                temp_vertices.push_back(vec4(v,1));
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
               res = fscanf(file,
                       "%f %f %f\n",
                       &f[0],
                       &f[1],
                       &f[2]);
                temp_faces.push_back(vec3(f));
            }

        }
        out_vertices = temp_vertices;
        out_faces = temp_faces;
    }
    else
    {
        cout << "Error file not found" << endl;
    }

    return load;
}
