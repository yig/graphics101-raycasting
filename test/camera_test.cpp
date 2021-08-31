#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "camera.h"

#include <iostream>
#include "debugging.h"

using namespace graphics101;

const real eps = 1e-5;

TEST_CASE( "Camera Perspective Test" ) {
    CameraPerspective cam( 0.3 );
    cam.setEye( vec3( 0,2,4 ) );
    cam.setFrameFromLookUp( vec3( 0, -0.44721359549995793, -0.89442719099991586 ), vec3( 0, 0.89442719099991586, -0.44721359549995793 ) );
    cam.setFilm( -0.58, 0.5, -0.3, 0.35 );
    
    const ray3 ray = cam.getRay( vec2( .3, -.4 ) );
    std::cout << "Your ray.p: " << ray.p << '\n';
    std::cout << "Your ray.d: " << ray.d << '\n';
    
    const vec3 p_true( 0, 2, 4 );
    const vec3 d_true( 0.3, -0.491935, -0.0894427 );
    std::cout << "Correct ray.p: " << ray.p << '\n';
    std::cout << "Correct ray.d: " << ray.d << '\n';
    
    // The ray originates from the eye.
    CHECK( length( ray.p - cam.eye() ) < eps );
    // Check against the precise values for this test case.
    CHECK( length( ray.p - p_true ) < eps );
    
    // The ray points in the direction of ground truth.
    CHECK( length( normalize( ray.d ) - normalize( d_true ) ) < eps );
}

TEST_CASE( "Camera Orthographic Test" ) {
    CameraOrthographic cam;
    cam.setEye( vec3( 3,1,-2 ) );
    cam.setFrameFromLookUp( vec3( -0.80178373, -0.26726124, -0.53452248 ), vec3( -0.22237479,  0.96362411, -0.14824986 ) );
    cam.setFilm( -0.5, 0.5, -0.5, 0.5 );
    
    const ray3 ray = cam.getRay( vec2( -.1, .2 ) );
    std::cout << "Your ray.p: " << ray.p << '\n';
    std::cout << "Your ray.d: " << ray.d << '\n';
    
    const vec3 p_true( 2.90006, 1.19272, -1.94644 );
    const vec3 d_true( -0.801784, -0.267261, -0.534522 );
    std::cout << "Correct ray.p: " << ray.p << '\n';
    std::cout << "Correct ray.d: " << ray.d << '\n';
    
    // The ray originates from the film plane point.
    CHECK( length( ray.p - p_true ) < eps );
    
    // The ray points in the look direction.
    CHECK( length( normalize( ray.d ) - normalize( cam.look() ) ) < eps );
    // Check against the precise values for this test case.
    CHECK( length( normalize( ray.d ) - normalize( d_true ) ) < eps );
}
