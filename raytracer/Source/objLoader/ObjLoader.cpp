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
        vector<vec4> & out_vector,
        vector<vec4> & out_norm)
{
    bool load = false;
    FILE * file = fopen(path, "r");
    vec3 v, n;

    vector<vec4> temp_vector;
    vector<vec4> temp_out;
    if (path != NULL)
    {
        while (1)
        {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;
            if (strcmp(lineHeader, "v") == 0)
            {
                fscanf(file,
                        "%f %f %f\n",
                        &v[0],
                        &v[1],
                        &v[2]);
                temp_vector.push_back(vec4(v,0));
            }
            else if (strcmp(lineHeader, "vn"))
            {
               fscanf(file,
                       "%f %f %f\n",
                       &n[0],
                       &n[1],
                       &n[2]);
                
            }
        }
    }
    else
    {
        cout << "Error file not found" << endl;
    }

    return load;
}
