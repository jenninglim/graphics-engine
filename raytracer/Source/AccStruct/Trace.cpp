#include "Trace.h"
#include <queue>
using namespace glm;

Trace::Trace()
{
    this->col= vec3(0);
    this->occ = 0.f;
}

Trace operator+(const Trace c1, const Trace c2)
{
    Trace t;
    t.col = c1.col + c2.col;
    t.occ = c1.occ + c2.occ;
    return t;
}


bool insideCube(vec3 p, float e) { return abs(p.x) < 1 + EPSILON && abs(p.y) < 1 +EPSILON && abs(p.z) < 1 + EPSILON;}

bool getPrecVoxel(Octree * root, const vec3 point, const float threshold, CloseVox &vox)
{
    bool found = false;
    int t_l_depth = glm::floor( - glm::log2(root->boxHalfSize[0])); // Lower depth
    int t_u_depth = glm::ceil( - glm::log2(root->boxHalfSize[0])); // Upper depth
    int c_depth = glm::round(-glm::log2(root->boxHalfSize[0]));
    
    // ASSUME t_u_depth > t_l_depth
    vec3 max = root->centre + root->boxHalfSize;
    vec3 min = root->centre - root->boxHalfSize;
    if (pointInsideAABB(point, min, max) && !found)
    {
        if (c_depth == t_u_depth)
        {
            vox.tree2 = root;
            return false;
        }
        if (c_depth == t_l_depth)
        {
            vox.tree = root;
            return true;
        }
        else if (root->type == NODE)
        {
            for (int i = 0; i < 8; i++)
            {
                found |= getPrecVoxel(&root->children[i], point, threshold, vox);
            }
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;

}

bool getVoxel(Octree * root, const vec3 point, const int depth, CloseVox &vox)
{
    bool found = false;
    int c_depth = - glm::log2(root->boxHalfSize[0]);
    vec3 min, max;
    max = root->centre + root->boxHalfSize * (1.f + EPSILON);
    min = root->centre - root->boxHalfSize * (1.f + EPSILON);
    if (pointInsideAABB(point, min, max) && !found)
    {
        if (depth == c_depth)
        {
            vox.tree = root;
            return true;
        }
        else if (root->type == NODE)
        {
            for (int i = 0; i < 8; i++)
            {
                found |= getVoxel(&root->children[i], point, depth, vox);
            }
        }
        else if (root->type == EMPTY)
        {
            return false;
        }
    }
    return found;
}

Cell averageNeighVoxel(Octree * root)
{
    Cell vox = Cell();
    int count = 0;
    for (int i = 0; i < NEIGHBOURS; i++)
    {
        if (root->neighbours[i] != NULL)
        {
            if (root->neighbours[i]->type != EMPTY)
            {
                vox = vox + *root->neighbours[i]->voxel;
                count++;
            }
        }
    }
    vox = (count == 0) ? vox : vox / (float) count;
    return vox;
}
