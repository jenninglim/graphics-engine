#include "glm/ext.hpp"
#include "Octree.h"
#include "Config.h"

#define MAX_DEPTH OCT_DEPTH

struct CloseVox{
    Octree * voxel;
    float diff;
};

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &voxel);
void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist);

float castShadowCone(Octree * root, vec3 point, Light l, float theta)
{
    Trace t;
    vec3 ab = vec3(l.position) - point;
    Cone r(vec4(point,0), glm::normalize(ab), theta);
    singleConeTrace(root, r,t, glm::min(MAX_DIST, glm::l2Norm(ab)));
    return (t.occlusion > 1) ? 1 : t.occlusion;
}

void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float start = 5e-3;
    float dist = start / tantheta;

    vec3 point = vec3(r.initial);
    float a = 0;
    float delta = 0;
    vec3 c = vec3(0);
    float weight;
    CloseVox vox;
    while (dist < maxDist)
    {
        vox.voxel=NULL;
        vox.diff = 20;
        point = vec3(r.initial) + dist * r.direction;
        if (ClosestVoxel(root, point, dist * tantheta, vox))
        {
            c = a * c + (1 - a) * vox.voxel->occlusion * vox.voxel->colour;
            weight = 1 / ( 1 +  3 *(dist+vox.diff)); 
            a += weight * (1 - a) * vox.voxel->occlusion;
            a = 1 - glm::pow(1 - a, dist / glm::l2Norm(vox.voxel->boxHalfSize));
            delta = glm::pow(dist,2);
        }
        else
        {
            delta = dist * tantheta;
        }
        dist += delta;

    }
    t.colour = c;
    t.occlusion = a;
}

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &vox)
{
    bool found = false;
    vec3 min, max;
    max = root->centre + root->boxHalfSize;
    min = root->centre - root->boxHalfSize;
    if (pointInsideAABB(point, min, max) && !found)
    {
        if (glm::l2Norm(root->boxHalfSize) < threshold)
        {
            if (glm::abs(glm::l2Norm(root->boxHalfSize) - threshold) < vox.diff)
            {
                vox.voxel = root;
                vox.diff = glm::abs(glm::l2Norm(root->boxHalfSize)-threshold);
                return true;
            }
        }
        else if (root->type == NODE)
        {
            for (int i = 0; i < 8; i++)
            {
                found |= ClosestVoxel(root->children[i], point, threshold, vox);
            }
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;
}
