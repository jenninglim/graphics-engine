#include "glm/ext.hpp"
#include "Octree.h"
#include "Config.h"

#define MAX_DEPTH OCT_DEPTH

using namespace glm;

struct CloseVox{
    Octree * voxel;
    float diff;
};

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &voxel);
void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist);

#define AMB_RAY 5
vec3 ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l)
{
    vec4 point(point1, 0);
    float theta = 0.5f;
    float deg = glm::pi<float>()/4;
    mat3 rotx(vec3(1,0,0), vec3(0,glm::cos(deg), glm::sin(deg)), vec3(0, -glm::sin(deg), glm::cos(deg)));
    mat3 rotz(vec3(cos(deg),sin(deg),0), vec3(-sin(deg),cos(deg),0), vec3(0,0,1));

    Cone r[AMB_RAY];

    r[0] = Cone(point, rotx * rotz * normal, theta); //0.03
    r[1] = Cone(point, rotx * glm::inverse(rotz) * normal, theta); //0.025
    r[2] = Cone(point, glm::inverse(rotx) * rotz *normal,theta);
    r[3] = Cone(point, glm::inverse(rotz) * glm::inverse(rotx)*normal,theta);
    r[4] = Cone(point, normal,theta);

    for (int i = 0; i < AMB_RAY; i++)
    {
            r[i].initial += 0.025f * vec4(r[i].direction,0);
    }

    Trace t;
    float acc = 0.f;
    vec3 colorAcc = vec3(0);
    for (int i = 0; i < AMB_RAY; i ++)
    {
        singleConeTrace(root, r[i], t, 2);
        acc += glm::pow(1-t.occlusion,3);
        colorAcc += t.colour;
    }
    acc /= AMB_RAY;
    colorAcc /=AMB_RAY;
    return vec3(acc);
}

float castShadowCone(Octree * root, vec3 point, Light l, float theta)
{
    Trace t;
    vec3 ab = vec3(l.position) - point;
    Cone r(vec4(point,0), glm::normalize(ab), theta);
    singleConeTrace(root, r,t, glm::min(MAX_DIST, glm::l2Norm(ab)));
    return t.occlusion;
}

void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float start = 0.001;
    float dist = start / tantheta;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;
    vec3 c = vec3(0);
    float weight;
    CloseVox vox;
    while (dist < maxDist)
    {
        vox.voxel = NULL;
        vox.diff = 20;
        point = vec3(r.initial) + dist * r.direction;
        if (ClosestVoxel(root, point, dist * tantheta, vox))
        {
            c = a * c + (1 - a) * vox.voxel->occlusion * vox.voxel->colour;
            weight = 1 / ( 1 +  3 *(dist+vox.diff)); 
            a += weight * (1 - a) * glm::pow(vox.voxel->occlusion,1);
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
    t.occlusion = (a > 1) ? 1 : a;
}

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &vox)
{
    bool found = false;
    vec3 min, max;
    max = root->centre + root->boxHalfSize;
    min = root->centre - root->boxHalfSize;
    float norm;
    if (pointInsideAABB(point, min, max) && !found)
    {
        norm = glm::l2Norm(root->boxHalfSize);
        if (abs(norm - threshold) < norm && norm < threshold)
        {
            if (glm::abs(norm - threshold) < vox.diff)
            {
                vox.voxel = root;
                vox.diff = glm::abs(norm-threshold);
                return true;
            }
        }
        else if (root->type == NODE)
        {
            for (int i = 0; i < 8; i++)
            {
                found |= ClosestVoxel(&root->children[i], point, threshold, vox);
            }
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;
}
