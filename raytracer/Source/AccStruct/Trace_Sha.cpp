#include "Trace.h"
#include "MyMath.h"

float singleShadowConeTrace(Octree * root, Cone r, float maxDist);

#define SHADOW_RAY 4
float castShadowCone(Octree * root, vec3 point, vec3 normal, vec3 dir, float maxDist)
{
    const float mix_delta = 0.2f;
    const float theta = 0.2f;

    float occ = 0;
    const vec3 e1 = findOthor(dir);
    const vec3 e2 = findOthor(dir, e1);
    
    const vec3 s1 = normalize(mix(dir, e1, mix_delta));
    const vec3 s2 = normalize(mix(dir, -e1, mix_delta));
#if (SHADOW_RAY == 4)
    const vec3 s3 = normalize(mix(dir, e2, mix_delta));
    const vec3 s4 = normalize(mix(dir, -e2, mix_delta));
#endif

    const vec3 offset = 0.00f * normal;
    const vec3 initial = point + offset;

    const float coneoffset = -0.01f;

    const Cone r[SHADOW_RAY] =
    { Cone(initial + coneoffset * s1, s1, theta),
      Cone(initial + coneoffset * s2, s2, theta)
#if (SHADOW_RAY == 4)
     ,Cone(initial + coneoffset * s3, s3, theta),
      Cone(initial + coneoffset * s4, s4, theta)
#endif
    };

    occ += singleShadowConeTrace(root, r[0], maxDist);
    occ += singleShadowConeTrace(root, r[1], maxDist);
#if (SHADOW_RAY == 4)
    occ += singleShadowConeTrace(root, r[2], maxDist);
    occ += singleShadowConeTrace(root, r[3], maxDist);
#endif

    occ /=SHADOW_RAY;
    return 1-occ;
}

float singleShadowConeTrace(Octree * root, Cone r, float maxDist)
{
    float occ = 0;
    float tantheta = glm::tan(r.theta);
    float start = 0.03;
    float dist = start;
    float weight = 0;

    vec3 point = vec3(r.initial);
    float a = 0.f;
    float delta = 0.f;

    // For accumulation
    CloseVox vox;
    Voxel average = Voxel();

    float radius = 0;

    for (int i = OCT_DEPTH -1; i > 0; i--)
    {
        radius = 1.f/ glm::pow(2,i);
        dist = radius/tantheta;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1 / (1+  10 * dist);

        if (!insideCube(point,0)) {
//            a += glm::pow(weight,2) * (1-a);
            break;
        }
        if (l2Norm(point - vec3(r.initial))< maxDist || occ > 1.f)
        {
            if (getVoxel(root, point, i, vox))
            {
                if (vox.tree->type != EMPTY)
                {
                    occ = vox.tree->interOcc(point);
                    a += glm::pow(weight,1) * (1 - a) * glm::pow(occ,1);
                }
                else // is EMPTY => Average
                {
                    average = averageNeighbourVox(vox.tree);
                    a += (1-occ) *average.occ;
                }
            }
        }
    }
    occ = (a > 1) ? 1 : a;
    return occ;
}
