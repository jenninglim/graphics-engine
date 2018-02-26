#ifndef MATERIAL_H
#define MATERIAL_H

using namespace glm;

class Material{

    public :
        vec3 colour;
        Material(vec3 colour)
            : colour(colour)
        {
        }
};

#endif
