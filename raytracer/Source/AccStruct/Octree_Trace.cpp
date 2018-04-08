#include "glm/ext.hpp"
#include "Octree.h"
#include "Config.h"
#include "MyMath.h"

#define MAX_DEPTH OCT_DEPTH

using namespace glm;

struct CloseVox{
    Octree * tree;
    float diff;
};

bool ClosestVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &voxel);
void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist);
bool ClosestVoxelLeaf(Octree * root, const vec3 point, CloseVox &vox);
bool insideCube(vec3 p, float e) { return abs(p.x) < 1 && abs(p.y) < 1 && abs(p.z);}

#define AMB_RAY 5
tex_t ambientOcclusion(Octree * root, vec3 point1, vec3 normal, Light l)
{
    Intersection inter;
    CloseVox vox;
    vec3 e1, e2;
    vec4 point(point1, 0);
    vox.diff = 20;
    float theta = DEG_TO_RAD(30);
    float deg = DEG_TO_RAD(20);

    static const mat3 rotx(vec3(1,0,0),
            vec3(0,glm::cos(deg), glm::sin(deg)),
            vec3(0, -glm::sin(deg), glm::cos(deg)));
    static const mat3 rotz(vec3(cos(deg),sin(deg),0),
            vec3(-sin(deg),cos(deg),0),
            vec3(0,0,1));
    
    // Find Basis Vectors
    e1 = findOthor(normal);
    e2 = findOthor(normal, e1);
    mat3 projMat = projMatr(e1,normal,e2);

    Cone r[AMB_RAY];
    r[0] = Cone(point, glm::inverse(projMat) * rotz * rotx * projMat * normal, theta); //0.03
    r[1] = Cone(point, glm::inverse(projMat) * rotz * inverse(rotx) * projMat* normal, theta); //0.025
    r[2] = Cone(point, glm::inverse(projMat) * inverse(rotz) * rotx  * projMat * normal,theta);
    r[3] = Cone(point, glm::inverse(projMat) * inverse(rotz) * inverse(rotx) *projMat*normal,theta);
    r[4] = Cone(point, normal,theta);

#if (AMB_RAY >1)
    for (int i = 0; i < AMB_RAY - 1; i++)
    {
        r[i].initial = r[i].initial + 0.02f * vec4(normal,0);
    }
#endif 
    
    Trace t;
    float acc = 0.f;
    vec3 colorAcc = vec3(0);
    for (int i = 0; i < AMB_RAY; i ++)
    {
        singleConeTrace(root, r[i], t, 2);
        acc += glm::pow(1-t.occlusion,2);
        colorAcc += t.colour;
    }
    acc /= AMB_RAY;
    colorAcc /= AMB_RAY;
    tex_t ret;
    ret.colour = colorAcc;
    ret.occ = acc;
    return ret;
}

float castShadowCone(Octree * root, vec3 point, Light l, float theta)
{
    Trace t;
    vec3 ab = vec3(l.position) - point;
    Cone r(vec4(point,0), glm::normalize(ab), theta);
    r.initial = r.initial + vec4(0.1f * r.direction,0);
    singleConeTrace(root, r,t, glm::min(MAX_DIST, glm::l2Norm(ab)));
    return t.occlusion;
}

void singleConeTrace(Octree * root, Cone r, Trace &t, float maxDist)
{
    float tantheta = glm::tan(r.theta);
    float start = 0.001;
    float dist = start;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;
    vec3 c = vec3(0);

    // For accumulation
    float weight, occ;
    CloseVox vox;

    // For intepolation.
    Octree * prev = NULL;
    Octree * next = NULL;
    float diff, fract, d;
    while (dist < maxDist)
    {
        vox.tree= NULL;
        vox.diff = 20;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1/(1+dist);

        if (!insideCube(point,0)) {
            a += glm::pow(weight,5) * (1-a);
            break;
        }
        if (ClosestVoxel(root, point, dist * tantheta, vox))
        {
            occ = trilinear<float>(point - vox.tree->centre + vox.tree->boxHalfSize,
                    vox.tree->children[7].voxel->occ,
                    vox.tree->children[6].voxel->occ,
                    vox.tree->children[5].voxel->occ,
                    vox.tree->children[4].voxel->occ,
                    vox.tree->children[3].voxel->occ,
                    vox.tree->children[2].voxel->occ,
                    vox.tree->children[1].voxel->occ,
                    vox.tree->children[0].voxel->occ
                    ); 

            c += vox.tree->voxel->colour * (vec3(1) -c)
                * (float) glm::pow(1.f-vox.tree->voxel->occ,2) * weight;
            a += glm::pow(weight,2) *(1 - a) * glm::pow(occ,1);
            a = 1 - glm::pow(1 - a, dist / glm::l2Norm(vox.tree->boxHalfSize)); // correction

            /*
            // Interpolate
            prev = next;
            next = vox.tree;
            if (prev != NULL)
            {
                d = l2Norm(
                    next->boxHalfSize - prev->boxHalfSize) * 0.5f;
                fract = (d - l2Norm(prev->boxHalfSize));

                if (fract <= 0)
                {
                }
                else
                {
                    fract /= (l2Norm(next->boxHalfSize) - l2Norm(prev->boxHalfSize));
                    a += weight * (next->occlusion - prev->occlusion) * fract;
                }
                //a = 1 - glm::pow(1 - a, l2Norm(prev->centre + d) / d);
            }
            */

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
        if (root->type == NODE && norm < threshold)

        {
            if (glm::abs(norm - threshold) < vox.diff)
            {
                vox.tree = root;
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

bool ClosestVoxelLeaf(Octree * root, const vec3 point, CloseVox &vox)
{
    bool found = false;
    vec3 min, max;
    max = root->centre + root->boxHalfSize;
    min = root->centre - root->boxHalfSize;
    if (pointInsideAABB(point, min, max) && !found)
    {
        if (root->type == NODE)
        {
            vox.tree = root;
            for (int i = 0; i < 8; i++)
            {
                found |= ClosestVoxelLeaf(&root->children[i], point, vox);
            }
        }
        else if (root->type == LEAF)
        {
            vox.tree = root;
            return true;
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;
}
