#include "Trace.h"
#include "MyMath.h"

#define MAX_DEPTH OCT_DEPTH

using namespace glm;

void singleAmbConeTrace(Octree * root, Cone r, Trace &t, float maxDist);

#define AMB_RAY 9
Trace ambientOcclusion(Octree * root, vec3 point, vec3 normal)
{
    const float max_dist = 0.5;
    Intersection inter;
    CloseVox vox;

    const float theta = 0.3;

    // Find Basis Vectors
    const vec3 e1 = findOthor(normal);
    const vec3 e2 = findOthor(normal, e1);

    // Find initial position
    const vec3 n_offset = 3 * normal * VOXEL_SIZE;
    const vec4 initial = vec4(point + n_offset,0);

    const float cone_offset = -0.01f;

    // Front Cone
    Cone r[AMB_RAY];

#if (AMB_RAY >=1)
    //Front Cone
    r[0] = Cone(initial + vec4(cone_offset * normal,0), normal, theta); //0.03
#endif

#if (AMB_RAY >=5)
    // Side Cone
    const vec3 s1 = normalize(mix(normal, e1, 0.5f));
    const vec3 s2 = normalize(mix(normal, -e1, 0.5f));
    const vec3 s3 = normalize(mix(normal, e2, 0.5f));
    const vec3 s4 = normalize(mix(normal, -e2, 0.5f));

    r[1] = Cone(initial + vec4(cone_offset * e1,0), s1, theta); //0.03
    r[2] = Cone(initial - vec4(cone_offset * e1,0), s2, theta); //0.03
    r[3] = Cone(initial + vec4(cone_offset * e2,0), s3, theta); //0.03
    r[4] = Cone(initial - vec4(cone_offset * e2,0), s4, theta); //0.03
#endif

    // Corner Cone?
#if (AMB_RAY == 9)
    const vec3 e3 = 0.5f * (e1 + e2);
    const vec3 e4 = 0.5f * (e1 - e2);

    const vec3 c1 = normalize(mix(normal, e3, 0.5f));
    const vec3 c2 = normalize(mix(normal, -e3, 0.5f));
    const vec3 c3 = normalize(mix(normal, e4, 0.5f));
    const vec3 c4 = normalize(mix(normal, -e4, 0.5f));

    r[5] = Cone(initial + vec4(cone_offset * e3,0), c1, theta); //0.03
    r[6] = Cone(initial - vec4(cone_offset * e3,0), c2, theta); //0.03
    r[7] = Cone(initial + vec4(cone_offset * e4,0), c3, theta); //0.03
    r[8] = Cone(initial - vec4(cone_offset * e4,0), c4, theta); //0.03
#endif

    Trace t;
    float acc = 0.f;
    vec3 colorAcc = vec3(0);
    for (int i = 0; i < AMB_RAY; i ++)
    {
        singleAmbConeTrace(root, r[i], t, max_dist);
        acc += glm::pow(1-t.occ,1);
        colorAcc += t.col;
    }
    acc /= AMB_RAY;
    colorAcc /= AMB_RAY;

    Trace ret;
    ret.col = colorAcc;
    ret.occ = acc;
    return ret;
}

void singleAmbConeTrace(Octree * root, Cone r, Trace &t, float maxDist)
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
    Cell averages = Cell();

    float radius = 0;

    for (int i = MAX_DEPTH - 1; i > 0; i--)
    {
        radius = 1.f/ glm::pow(2,i);
        dist = radius/tantheta;
        point = vec3(r.initial) + dist * r.direction;
        weight = 1 / (1+ 1 *dist);

        if (!insideCube(point,0)) {
            a += glm::pow(weight,1) * (1-a);
            break;
        }
        if (l2Norm(point - vec3(r.initial)) < maxDist)
        {
            if (getVoxel(root, point, i, vox))
            {
                if (vox.tree->type == LEAF)
                {
                   a += glm::pow(weight,1) * (1 - a);
                }
                else if (vox.tree->type == NODE)
                {
                    //col = vox.tree->voxel->col;
                    averages = vox.tree->interVox(point);
                    c += (vec3(1) - c) * averages.col * averages.irrad;
                }
                else
                {
                    Cell average = averageNeighVoxel(vox.tree);
                    c +=  0.25f * (vec3(1) - c) * average.col * average.irrad;
                }
            }
        }
        else
        {
            break;
        }
    }
    t.col = c;
    t.occ = (a > 1) ? 1 : a;
}
