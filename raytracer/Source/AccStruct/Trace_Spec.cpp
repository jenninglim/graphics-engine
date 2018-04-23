#include "Trace.h"

vec3 singleSpecCone(Octree * root, Cone r, float maxDist);

#define SPEC_RAY 1
vec3 castSpecCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist)
{
    const float theta = 0.3f;
    vec3 rad(0);

    const vec3 offset = VOXEL_SIZE * normal;
    const vec3 initial = point ;//+ offset;

    const float coneoffset = -0.01f;

    const Cone r[SPEC_RAY] =
    {
        Cone(initial, dir, theta)
    };
    rad += singleSpecCone(root, r[0], maxDist);

    rad /= SPEC_RAY;
    return rad;
}

vec3 singleSpecCone(Octree * root, Cone r, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float dist = 0;
    float weight = 0;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;

    // For accumulation
    CloseVox vox;
    float radius = 0;
    float rad = 0;
    Voxel average = Voxel();
    
   for (int i = OCT_DEPTH - 1; i > 0; i--)
    {
        radius = 1.f/ glm::pow(2,i);
        dist = radius/tantheta;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1.f / (1.f + 10 * dist);

        if (!insideCube(point,0)) {
           // rad -= glm::pow(weight,1) * (1-rad);
            break;
        }
        if (l2Norm(point - vec3(r.initial))< maxDist)
        {
            if (getVoxel(root, point, i, vox))
            {
                if (vox.tree->type != EMPTY)
                {
                    rad +=glm::pow(weight,1)* (1-rad) *vox.tree->voxel->irrad;
                    a += glm::pow(weight,1) * (1 - a) * glm::pow(vox.tree->voxel->occ,1);
                }
                else // is EMPTY => Average
                {
                    average = averageNeighbourVox(vox.tree);
                    rad +=glm::pow(weight,1)* (1-rad) *average.irrad;
                }
            }
            else
            {

            }
        }
        else
        {
            //a = 1.;
        }
    }
    return vec3(rad);
}
