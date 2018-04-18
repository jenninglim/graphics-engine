#include "Trace.h"

vec3 singleSpecCone(Octree * root, Cone r, float maxDist);

#define SPEC_RAY 1
vec3 castSpecCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist)
{
    const float theta = 0.3f;
    vec3 rad(0);

    const vec3 offset =  5 * VOXEL_SIZE * normal;
    const vec3 initial = point + offset;

    const float coneoffset = -0.03f;

    vec3 reflect_dir = reflect(dir, normal);

    const Cone r[SPEC_RAY] =
    {
        Cone(initial + coneoffset * reflect_dir, reflect_dir, theta)
    };

    rad += singleSpecCone(root, r[0], maxDist);

    rad /= SPEC_RAY;
    return rad;
}

vec3 singleSpecCone(Octree * root, Cone r, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float start = 0.03;
    float dist = start;
    float weight = 0;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;

    // For accumulation
    CloseVox vox;
    float radius = 0;
    float rad = 0;

    for (int i = OCT_DEPTH; i > 0; i--)
    {
        radius = 1.f/ glm::pow(2,i);
        dist = radius/tantheta;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1 / (1+ dist);

        if (!insideCube(point,0)) {
            rad -= glm::pow(weight,1) * (1-rad);
            break;
        }
        if (l2Norm(point - vec3(r.initial))< maxDist)
        {
            if (getVoxel(root, point, i, vox))
            {
                rad +=glm::pow(weight,1)* vox.tree->voxel->irrad  * (1- vox.tree->voxel->occ) * (1 -a);
                a += glm::pow(weight,1) * (1 - a) * glm::pow(vox.tree->voxel->occ,1);
            }
        }
    }
    return vec3(rad);
}
