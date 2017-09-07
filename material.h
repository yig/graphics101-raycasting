#ifndef __material_h__
#define __material_h__

#include "types.h"

namespace graphics101 {

struct Material {
    vec3 color_ambient{ 0., 0., 0. };
    vec3 color_diffuse{ 1., 1., 1. };
    vec3 color_specular{ 1., 1., 1. };
    real shininess{ 10. };
    
    bool reflective{ false };
    vec3 color_reflect{ 1., 1., 1. };
    
    bool refractive{ false };
    vec3 color_refract{ 1., 1., 1. };
    real index_of_refraction{ 1.0 };
    
    bool use_diffuse_texture{ false };
    string diffuse_texture;
};

}

#endif /* __material_h__ */
