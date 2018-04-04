#include "glm/ext.hpp"
#include "Octree.h"
#include "Config.h"

#define MAX_DEPTH OCT_DEPTH
#define MAX_DIST 5

bool ClosestVoxel(Octree * root, vec3 point, float threshold, Octree * & voxel);
void singleConeTrace(Octree * root, Cone r, Trace &t);

float castShadowCone(Octree * root, vec3 point, Light l, float theta)
{
    Trace t;
    Cone r(vec4(point,0), glm::normalize(vec3(l.position) - point), theta);
    singleConeTrace(root, r,t);
    return (t.occlusion > 1) ? 1 : t.occlusion;
}

void singleConeTrace(Octree * root, Cone r, Trace &t)
{
    float tantheta = glm::tan(r.theta);
    float start = 1e-2;
    float dist = start / tantheta;
    vec3 point = vec3(r.initial);
    Octree * voxel = NULL;

    float a = 0;
    float delta = 0;
    vec3 c = vec3(0);
    while (dist < MAX_DIST)
    {
        point = vec3(r.initial) + dist * r.direction;
        if (ClosestVoxel(root, point, dist * tantheta, voxel)) {

            c = a * c + (1 - a) * voxel->occlusion * voxel->colour;
            a += (1 - a) * voxel->occlusion;
            delta = glm::pow(dist,2);
            a = 1 - glm::pow(1 -a, delta / glm::l2Norm(voxel->boxHalfSize));
            dist += delta;
        }
        else
        {
            delta = glm::pow(dist,2);
            dist += delta;
        }
    }
    t.colour = c;
    t.occlusion = a;
}

bool ClosestVoxel(Octree * root, vec3 point, float threshold, Octree * &voxel)
{
    bool found = false;
    if (root->bv.pointInside(point))
    {
        if (glm::l2Norm(root->boxHalfSize) < threshold)
        {
            voxel = root;
            return true;
        }
        else if (root->type == NODE)
        {
            for (int i = 0; i < 8; i++)
            {
                found |= ClosestVoxel(root->children[i], point, threshold, voxel);
            }
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;
}
