#include "Trace.h"

float castShadowCone(Octree * root, vec3 point, Light direction, float l)
{
    /*
    Trace t;
    vec3 ab = vec3(l.position) - point;
    Cone r(vec4(point,0), glm::normalize(ab), theta);
    r.initial = r.initial + vec4(0.1f * r.direction,0);
    singleConeTrace(root, r,t, 1);
    return t.occ;
    */
    return 0.f;
}

void singleShadowConeTrace(Octree * root, Cone r, Trace &t, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float start = 0.03;
    float dist = start;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;
    vec3 c = vec3(0);

    // For accumulation
    float weight, occ;
    vec3 col(0);
    CloseVox vox;
    while (dist < maxDist)
    {
        vox.tree= NULL;
        vox.diff = 20;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1/(1+10 *dist);

        if (!insideCube(point,0)) {
            a += glm::pow(weight, 10) * (1-a);
            break;
        }
        if (ClosestVoxel(root, point, dist * tantheta, vox))
        {
            occ = vox.tree->voxel->occ;
            col = vox.tree->interCol(point);
            c += col * (vec3(1) - c)
                * (1.f - occ) * (float) glm::pow(weight,2);
            a += glm::pow(weight,2) * (1 - a) * glm::pow(occ,1);

           delta = glm::pow(dist,2);
        }
        else
        {
            delta = dist * tantheta;
        }
        dist += delta;
    }
    t.colour = c;
    t.occ = (a > 1) ? 1 : a;
}
