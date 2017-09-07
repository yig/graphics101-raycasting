#ifndef __scene_h__
#define __scene_h__

#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map> // hash table aka std::unordered_map

#include <QImage>

#include "types.h"
#include "material.h"
#include "light.h"
#include "shape.h"
#include "camera.h"

namespace graphics101 {

class Scene {
public:
    bool parse( const std::string& path_to_file );
    
    QImage render( int width, int height ) {
        QImage result( width, height, QImage::Format_ARGB32 );
        render( result );
        return result;
    }
    void render( QImage& into_image );

protected:
    // Returns true and fills out `hit_out` if the ray intersects an object with t>=0.
    // Returns false otherwise.
    bool closestIntersection( const ray3& ray, Intersection& hit_out ) const;
    // Returns the color along the ray.
    // `ray` is the ray along which the intersection will be obtained.
    // max_recursion is the maximum number of times to recurse.
    vec3 rayColor( const ray3& ray, int max_recursion = 0 ) const;
    
public:
    typedef std::shared_ptr< Camera > CameraPtr;
    typedef std::shared_ptr< Shape > ShapePtr;

    CameraPtr camera;
    std::vector< ShapePtr > shapes;
    std::vector< Light > lights;
    std::unordered_map< std::string, QImage > textures;
    
    std::unordered_map< std::string, mat4 > transforms;
    std::unordered_map< std::string, Material > materials;
};

}

#endif /* __scene_h__ */
