#include "Trace.h"
#include "MyMath.h"

#define MAX_DEPTH OCT_DEPTH

using namespace glm;

void singleAmbConeTrace(Octree * root, Cone r, Trace &t, float maxDist);

#define AMB_RAY 5
Trace ambientOcclusion(Octree * root, vec3 point, vec3 normal, Light l)
{
    Intersection inter;
    CloseVox vox;
    vox.diff = 20;

    const float theta = 0.3125;

    // Find Basis Vectors
    const vec3 e1 = findOthor(normal);
    const vec3 e2 = findOthor(normal, e1);

    // Find initial position
    const vec3 n_offset = normal * VOXEL_SIZE;
    const vec4 initial = vec4(point + n_offset * normal,0);

    const float cone_offset =0.01f;

    // Front Cone
    Cone r[AMB_RAY];
    //Front Cone
    r[0] = Cone(initial + vec4(cone_offset * normal,0), normal, theta); //0.03

    // Side Cone
    const vec3 s1 = mix(normal, e1, 0.5f);
    const vec3 s2 = mix(normal, -e1, 0.5f);
    const vec3 s3 = mix(normal, e2, 0.5f);
    const vec3 s4 = mix(normal, -e2, 0.5f);

    r[1] = Cone(initial + vec4(cone_offset * e1,0), s1, theta); //0.03
    r[2] = Cone(initial - vec4(cone_offset * e1,0), s2, theta); //0.03
    r[3] = Cone(initial + vec4(cone_offset * e2,0), s3, theta); //0.03
    r[4] = Cone(initial - vec4(cone_offset * e2,0), s4, theta); //0.03

    // Corner Cone?
#if (AMB_RAY == 9)
    const vec3 e3 = 0.5f * (e1 + e2);
    const vec3 e4 = 0.5f * (e1 - e2);

    const vec3 c1 = mix(normal, e3, 0.5f);
    const vec3 c2 = mix(normal, -e3, 0.5f);
    const vec3 c3 = mix(normal, e4, 0.5f);
    const vec3 c4 = mix(normal, -e4, 0.5f);

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
        singleAmbConeTrace(root, r[i], t, 1);
        acc += glm::pow(1-t.occ,2);
        colorAcc += t.colour;
    }
    acc /= AMB_RAY;
    colorAcc /= AMB_RAY;

    Trace ret;
    ret.colour = colorAcc;
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
