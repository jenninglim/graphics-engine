#include "Trace.h"

void singleShadowConeTrace(Octree * root, Cone r, float &occ, float maxDist);

float castShadowCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist)
{
    float occ;
    vec4 start = vec4(point + 0.03f * normal - 0.03f *dir ,0);
    Cone r(start, dir, 0.3);
    singleShadowConeTrace(root, r, occ, maxDist);
    return 1-occ;
}

void singleShadowConeTrace(Octree * root, Cone r, float &occ, float maxDist)
{
    const float tantheta = glm::tan(r.theta);
    float dist = 0.03;
    vec3 point;
    float delta = 0.f;

    // For accumulation
    float weight;
    CloseVox vox;
    occ = 0;
    while (dist < maxDist && occ < 1)
    {
        vox.tree= NULL;
        vox.diff = 20;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1/(1+10 *dist);
        if (!insideCube(point,0)) {
            occ += glm::pow(weight, 10) * (1-occ);
            break;
        }
        if (ClosestVoxel(root, point, dist * tantheta, vox))
        {
            occ += (1 - occ) * glm::pow(vox.tree->voxel->occ,1);
            delta = glm::pow(dist,2);
        }
        else
        {
            delta = dist * tantheta;
        }
        dist += delta;
    }
    occ = (occ > 1) ? 1 : occ;
}
