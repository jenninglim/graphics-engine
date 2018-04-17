#include "Trace.h"

float singleSpecCone(Octree * root, Cone r, float maxDist);

#define SPEC_RAY 1
float castSpecCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist)
{
    const float theta = 0.3f;
    float rad = 0;

    const vec3 offset = 0.00f * normal;
    const vec3 initial = point + offset;

    const float coneoffset = -0.01f;

    vec3 reflect_dir = reflect(dir, normal);

    const Cone r[SPEC_RAY] =
    { Cone(initial + coneoffset * reflect_dir, reflect_dir, theta)
    };

    rad += singleSpecCone(root, r[0], maxDist);

    rad /= SPEC_RAY;
    return rad;
}

float singleSpecCone(Octree * root, Cone r, float maxDist)
{
    float rad = 0;
    float tantheta = glm::tan(r.theta);
    float start = 0.03;
    float dist = start;
    float weight = 0;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;
    vec3 c = vec3(0);

    // For accumulation
    vec3 col(0);
    CloseVox vox;

    float radius = 0;

    for (int i = OCT_DEPTH - 1; i > 0; i--)
    {
        radius = 1.f/ glm::pow(2,i);
        dist = radius/tantheta;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1 / (1+ dist);

        if (!insideCube(point,0)) {
            //a += glm::pow(weight,10) * (1-a);
            break;
        }
        if (l2Norm(point - vec3(r.initial))< maxDist)
        {
            if (getVoxel(root, point, i, vox))
            {
                rad = vox.tree->voxel->irrad;
                col = vox.tree->voxel->col;
                c += col * (vec3(1) - c);
                a += glm::pow(weight,1) * (1 - a) * glm::pow(rad,1);
            }
        }
    }
    return rad;
}
