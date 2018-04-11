#include "Trace.h"

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
