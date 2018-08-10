#include "shape.h"

#include "debugging.h"

#include <vector>
#include <cmath> // std::sqrt()

namespace graphics101 {

bool Sphere::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).
    
    return false;
}

bool Plane::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).
    
    return false;
}

bool Cylinder::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return false;
}

bool Cone::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return false;
}

bool Cube::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return false;
}

bool Mesh::rayIntersect( const ray3& ray, Intersection& hit_out ) const {
    // Your code goes here.

    // Remember that this intersection occurs in object-space.
    // You convert the world-space ray to object-space by multipling transformInverse() * ray.p and ray.d.
    // When filling out `hit_out`, don't forget to return the resulting position and normal in world-space.
    // For the normal, that means left-multiplying the object-space normal by transpose( transformInverse() ).

    return false;
}

}
