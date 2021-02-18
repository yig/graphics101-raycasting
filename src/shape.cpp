#include "shape.h"

#include "debugging.h"

#include <vector>
#include <cmath> // std::sqrt()

using namespace glm;

namespace graphics101 {

Intersection Plane::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.
    
    return result;
}

Intersection Sphere::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.
    
    return result;
}

Intersection Cylinder::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.

    return result;
}

Intersection Cone::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.

    return result;
}

Intersection Cube::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.

    return result;
}

Intersection Mesh::rayIntersect( const ray3& ray ) const {
    // The Intersection constructor sets `.valid = false`
    Intersection result;
    
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multiplying `transformInverse()` times `ray.p` and `ray.d`.
    // When filling out `result`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means multiplying `transpose( transformInverse() )` times the object-space normal.

    return result;
}

}
