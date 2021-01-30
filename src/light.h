#ifndef __light_h__
#define __light_h__

namespace graphics101 {

class Light {
public:
    typedef graphics101::vec3 vec3;
    
    vec3 position{ 0., 0., 0. };
    vec3 color{ 1., 1., 1. };
    vec3 color_ambient{ 1., 1., 1. };
};

}

#endif /* __light_h__ */
