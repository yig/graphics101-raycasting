#ifndef __types_h__
#define __types_h__

// We could just do this to get everything glm supports,
// but we're only using a subset so no need to slow down compile times.
// #include "glm/glm.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp" // glm::mat4
#include "glm/mat3x3.hpp" // glm::mat4
#include "glm/mat4x4.hpp" // glm::mat4

#include <vector>
#include <string>

#include <cassert>

#include <cmath> // std::sqrt()
#include <algorithm> // std::min(), std::max()
#include <limits> // infinity

namespace graphics101 {
    typedef double real;
    typedef glm::dvec2 vec2;
    typedef glm::dvec3 vec3;
    typedef glm::dvec4 vec4;
    typedef glm::ivec2 ivec2;
    typedef glm::ivec3 ivec3;
    typedef glm::ivec4 ivec4;
    typedef glm::dmat2 mat2;
    typedef glm::dmat3 mat3;
    typedef glm::dmat4 mat4;
    
    struct ray3
    {
        // point
        vec3 p;
        // direction
        vec3 d;
        
        ray3( const vec3& a_p, const vec3& a_d ) : p( a_p ), d( a_d ) {}
    };
    
    typedef ivec3 Triangle;
    typedef std::vector< Triangle > TriangleVec;
    
    typedef std::string string;
    
    // Constants
    const real infinity = std::numeric_limits<real>::infinity();
    const real pi = 3.14159265358979323846264338327950288;
}

#endif /* __types_h__ */
