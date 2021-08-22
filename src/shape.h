#ifndef __shape_h__
#define __shape_h__

#include "types.h"
#include "material.h"
#include <string>

namespace graphics101 {

struct Intersection {
    bool valid{ false }; // An intersection occurs only if `valid` is true.
    real t{ infinity }; // t along the ray
    vec3 position{ -31337, -31337, -31337 }; // world-space position
    vec3 normal{ -31337, -31337, -31337 }; // world-space normal
    vec2 texCoord{ -31337, -31337 }; // texture coordinates
    Material material;
};

class Shape {
public:
    virtual ~Shape() {}
    
    // If there was no intersection, the returned `Intersection.valid = false`.
    // If there was an intersection, the returned `Intersection.valid = true`
    // and the Intersection's fields are filled out.
    // This is the only function needed by a raytracer.
    // Subclasses override this to define the shape.
    virtual Intersection rayIntersect( const ray3& ray ) const = 0;
    
// Public but only used by subclasses or someone creating a scene.
public:
    const mat4& transform() const { return m_xform; }
    const mat4& transformInverse() const { return m_xform_inverse; }
    void setTransform( const mat4& a_xform ) { m_xform = a_xform; m_xform_inverse = glm::inverse( m_xform ); }
    void setTransform( const mat4& a_xform, const mat4& a_xform_inverse ) { m_xform = a_xform; m_xform_inverse = a_xform_inverse; }
    
    const std::string& transformName() const { return m_xform_name; }
    void setTransformName( const std::string& a_xform_name ) { m_xform_name = a_xform_name; }
    
    const Material& material() const { return m_material; }
    Material& material() { return m_material; }
    void setMaterial( const Material& a_material ) { m_material = a_material; }
    
    const std::string& materialName() const { return m_material_name; }
    std::string& materialName() { return m_material_name; }
    void setMaterialName( const std::string& a_material_name ) { m_material_name = a_material_name; }
    
private:
    // Identity matrix by default
    mat4 m_xform{1};
    mat4 m_xform_inverse{1};
    std::string m_xform_name;
    
    Material m_material;
    std::string m_material_name;
};

class Plane : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;
};

class Sphere : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;
};

class Cylinder : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;
};

class Cone : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;
};

class Cube : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;
};

class Mesh : public Shape {
public:
    Intersection rayIntersect( const ray3& ray ) const override;

    // Clears the mesh and loads the OBJ file at `path`.
    // Returns true upon success and false otherwise.
    bool loadFromOBJ( const std::string& path );

private:
    std::vector< vec3 > positions;
    TriangleVec face_positions;
    
    std::vector< vec3 > normals;
    TriangleVec face_normals;
    
    std::vector< vec2 > texcoords;
    TriangleVec face_texcoords;
};

}

#endif /* __shape_h__ */
