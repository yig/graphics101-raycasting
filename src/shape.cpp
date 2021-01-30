#include "shape.h"

#include "debugging.h"

#include <vector>
#include <cmath> // std::sqrt()

using namespace glm;

namespace graphics101 {

bool Sphere::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).
    
    return result;
}

Intersection Plane::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).
    
    return result;
}

Intersection Cylinder::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return result;
}

Intersection Cone::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return result;
}

Intersection Cube::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return result;
}

Intersection Mesh::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by left-multiplying ray.p and ray.d by transformInverse().
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return result;
}

}
