#include "scene.h"

// Include all of glm here.
#include "glm/glm.hpp"

#include "debugging.h"

namespace graphics101 {

void Scene::render( QImage& into_image ) {
    // Your code goes here.

    // Iterate over the pixels of the image. For each pixel:
    // 1. Use camera->getPixelUV() and camera->getRay() to create a ray3.
    // 2. Call rayColor() to get a vec3 color for that ray.
    // 3. Use into_image.setPixel() to set the pixel color.

}

bool Scene::closestIntersection( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Iterate over all the shapes, calling rayIntersect() on each.
    // In C++, a nice iterator for loop can be written as:
    // for( const ShapePtr shape: shapes ) { ... shape->rayIntersect( ... ) ... }

    return false;
}
vec3 Scene::rayColor( const ray3& ray, int max_recursion ) const {
    // Your code goes here.

    // In this ray casting assignment, you will simply call closestIntersection(),
    // and, if there is one, return the .material.color_diffuse;
    // Otherwise, return black (0,0,0).

    assert( max_recursion >= 0 );

    const real eps = 1e-7;

    vec3 c( 0,0,0 );

    return c;
}

}
