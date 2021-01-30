#ifndef __camera_h__
#define __camera_h__

#include <cassert>

#include "types.h"

namespace graphics101 {

class Camera
{
public:
    Camera() {
        setEye( vec3(0,0,0) );
        setFrameFromLookUp( vec3(0,0,-1), vec3(0,1,0) );
        setFilm( -1, 1, -1, 1 );
    }
    // In C++, we need this magic incantation when using polymorphism.
    // More: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
    virtual ~Camera() {}
    
    void setEye( const vec3& eye ) {
        e = eye;
    }
    
    void setFrameFromLookUp( const vec3& look, const vec3& up ) {
        v = up;
        w = -look;
        u = cross( v, w );
    }
    
    void setFilm( real a_left, real a_right, real a_bottom, real a_top ) {
        film_left = a_left;
        film_right = a_right;
        film_bottom = a_bottom;
        film_top = a_top;
    }
    
    const vec3& eye() const { return e; }
    vec3 look() const { return -w; }
    const vec3& up() const { return v; }
    const vec3& right() const { return u; }
    
    // Subclasses implement this:
    virtual ray3 getRay( const vec2& uv ) const = 0;
    
    vec2 getPixelUV( const vec2& xy, int image_width, int image_height ) const {
        assert( image_width > 0 );
        assert( image_height > 0 );
        
        return vec2(
            film_left + (film_right-film_left)*( xy.x + 0.5 )/image_width,
            // Use 1 - y, because QImage x,y is the top-left.
            film_bottom + (film_top-film_bottom)*( 1.0 - ( xy.y + 0.5 )/image_height )
            );
    }
    
    // Returns the width and height for an image with the same aspect ratio
    // as the camera film, and whose longest side has `long_edge_pixels`.
    // `long_edge_pixels` must be a positive number.
    // The width and height output are both guaranteed to be >= 1.
    void imageDimensionsWithLongEdge( int long_edge_pixels, int& width_out, int& height_out ) const;
    
protected:
    // Center point.
    vec3 e{0};
    // right, up, -look
    vec3 u{1,0,0},v{0,1,0},w{0,0,1};
    
    // Film parameters.
    real film_left, film_right, film_bottom, film_top;
};

class CameraPerspective : public Camera {
public:
    CameraPerspective( real focal_length )
    {
        setPerspectiveParameters( focal_length );
    }
    
    void setPerspectiveParameters( real a_focal_length ) {
        assert( a_focal_length > 0. );
        focal_length = a_focal_length;
    }
    
    ray3 getRay( const vec2& uv ) const override;
    
private:
    real focal_length;
};

class CameraOrthographic : public Camera {
public:
    ray3 getRay( const vec2& uv ) const override;
};

}

#endif /* __camera_h__ */
