#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "camera.h"
#include "scene.h"

#include <cmath> // fabs()
#include <iostream>
#include "debugging.h"

namespace graphics101 {
class DummyShape : public Shape {
public:
    DummyShape( const Intersection& intersection_ )
        : intersection( intersection_ ) {}
    
    Intersection rayIntersect( const ray3& ray ) const {
        return intersection;
    }
    
    Intersection intersection;
};

bool operator==( const Material& lhs, const Material& rhs ) {
    return
        lhs.color_ambient == rhs.color_ambient &&
        lhs.color_diffuse == rhs.color_diffuse &&
        lhs.color_specular == rhs.color_specular &&
        lhs.shininess == rhs.shininess &&
        lhs.reflective == rhs.reflective &&
        lhs.color_reflect == rhs.color_reflect &&
        lhs.refractive == rhs.refractive &&
        lhs.color_refract == rhs.color_refract &&
        lhs.index_of_refraction == rhs.index_of_refraction &&
        lhs.use_diffuse_texture == rhs.use_diffuse_texture &&
        lhs.diffuse_texture == rhs.diffuse_texture
        ;
};
bool operator==( const Intersection& lhs, const Intersection& rhs ) {
    return
        lhs.valid == rhs.valid &&
        lhs.t == rhs.t &&
        lhs.position == rhs.position &&
        lhs.normal == rhs.normal &&
        lhs.texCoord == rhs.texCoord &&
        lhs.material == rhs.material
        ;
}

}

using namespace graphics101;

const real eps = 1e-5;

TEST_CASE( "Scene closestIntersection() Test" ) {
    Scene scene;
    
    // The default camera is at the origin looking along -z.
    scene.camera = std::make_shared< CameraPerspective >( 0.3 );
    
    Intersection isect_truth;
    /* UPDATE: We can't test t<0, because raycasting asserts that valid intersections have t>=0.
               If we wanted to test this, we would need to test that the shapes
               return t>=0 whenever valid.
    // Magenta means not ignoring t<0.
    {
        Intersection isect;
        isect.valid = true;
        isect.t = -1;
        isect.position = vec3( 0,0,1 );
        isect.normal = vec3( 0,0,-1 );
        isect.material.color_ambient = vec3( 0,0,0 );
        isect.material.color_diffuse = vec3( .5,0,.5 );
        isect.material.color_specular = vec3( 0,0,0 );
        scene.shapes.emplace_back( std::make_shared< DummyShape >( isect ) );
    }
    */
    // Red means not ignoring .valid = false.
    {
        Intersection isect;
        isect.valid = false;
        isect.t = .1;
        isect.position = vec3( 0,0,-.1 );
        isect.normal = vec3( 0,0,1 );
        isect.material.color_ambient = vec3( 0,0,0 );
        isect.material.color_diffuse = vec3( .5,0,0 );
        isect.material.color_specular = vec3( 0,0,0 );
        scene.shapes.emplace_back( std::make_shared< DummyShape >( isect ) );
    }
    // Yellow is wrong, because it's farther than the green shape.
    {
        Intersection isect;
        isect.valid = true;
        isect.t = 3;
        isect.position = vec3( 0,0,-3 );
        isect.normal = vec3( 0,0,1 );
        isect.material.color_ambient = vec3( 0,0,0 );
        isect.material.color_diffuse = vec3( .5,.5,0 );
        isect.material.color_specular = vec3( 0,0,0 );
        scene.shapes.emplace_back( std::make_shared< DummyShape >( isect ) );
    }
    // Correct intersection is green.
    {
        Intersection isect;
        isect.valid = true;
        isect.t = 1;
        isect.position = vec3( 0,0,-1 );
        isect.normal = vec3( 0,0,1 );
        isect.material.color_ambient = vec3( 0,0,0 );
        isect.material.color_diffuse = vec3( 0,.5,0 );
        isect.material.color_specular = vec3( 0,0,0 );
        scene.shapes.emplace_back( std::make_shared< DummyShape >( isect ) );
        // This is the correct one.
        isect_truth = isect;
    }
    // Blue is wrong, because it's farther than the green shape. It's the last shape in the list.
    {
        Intersection isect;
        isect.valid = true;
        isect.t = 2;
        isect.position = vec3( 0,0,-2 );
        isect.normal = vec3( 0,0,1 );
        isect.material.color_ambient = vec3( 0,0,0 );
        isect.material.color_diffuse = vec3( 0,0,.5 );
        isect.material.color_specular = vec3( 0,0,0 );
        scene.shapes.emplace_back( std::make_shared< DummyShape >( isect ) );
    }
    
    // The default light is at the origin with color white.
    scene.lights.push_back( Light() );
    
    // The shape with t = 1 should be the closest intersection.
    CHECK( scene.closestIntersection( ray3( vec3(0,0,0), vec3(0,0,-1) ) ) == isect_truth );
    
    // The color returned by rayColor() should be the same as
    // the diffuse color.
    std::cout << scene.rayColor( ray3( vec3(0,0,0), vec3(0,0,-1) ) ) << '\n';
    CHECK( length( scene.rayColor( ray3( vec3(0,0,0), vec3(0,0,-1) ) ) - isect_truth.material.color_diffuse ) < eps );
    
    const Image result = scene.render( 1,1 );
    CHECK( result.width() == 1 );
    CHECK( result.height() == 1 );
    CHECK( result.pixel(0,0).r == 0 );
    CHECK( fabs( result.pixel(0,0).g/255. - .5 ) < 2./255. );
    CHECK( result.pixel(0,0).b == 0 );
    CHECK( result.pixel(0,0).a == 255 );
}
