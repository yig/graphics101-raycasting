#ifndef __scene_h__
#define __scene_h__

#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map> // hash table aka std::unordered_map

#include "image.h"
#include "types.h"
#include "material.h"
#include "light.h"
#include "shape.h"
#include "camera.h"

namespace graphics101 {

class Scene {
public:
    bool parse( const std::string& path_to_file );
    
    Image render( int width, int height ) {
        Image result( width, height );
        render( result );
        return result;
    }
    void render( Image& into_image );

    // Returns an `Intersection` with `.valid = true` and fills out the rest of its
    // fields if the ray intersects an object with t>=0.
    // Returns an `Intersection` with `.valid = false` otherwise.
    Intersection closestIntersection( const ray3& ray ) const;
    // Returns the color along the ray.
    // `ray` is the ray along which the intersection will be obtained.
    // max_recursion is the maximum number of times to recurse.
    vec3 rayColor( const ray3& ray, int max_recursion = 0 ) const;
    
    typedef std::shared_ptr< Camera > CameraPtr;
    typedef std::shared_ptr< Shape > ShapePtr;

    CameraPtr camera;
    std::vector< ShapePtr > shapes;
    std::vector< Light > lights;
    std::unordered_map< std::string, Image > textures;
    
    std::unordered_map< std::string, mat4 > transforms;
    std::unordered_map< std::string, Material > materials;
};

}

#endif /* __scene_h__ */
