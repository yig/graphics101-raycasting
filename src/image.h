#ifndef __image_h__
#define __image_h__

#include <cstdint> // uint8_t
#include <string>
#include <vector>

namespace graphics101
{
struct ColorRGBA8 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    
    // When specifying a color, alpha defaults to opaque.
    ColorRGBA8( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 )
        : r( red ), g( green ), b( blue ), a( alpha ) {}
    // By default, a color is transparent black.
    ColorRGBA8() : r(0), g(0), b(0), a(0) {}
};

class Image
{
public:
    // An empty constructor creates an empty image with width and height set to 0
    // no data.
    Image() = default;
    // Create an image with undefined contents. Use .fill() or the next constructor to set a color.
	Image( int width, int height );
	// Create an image filled with the given pixel color.
	Image( int width, int height, ColorRGBA8 color );
	// Create an image from memory.
	Image( const ColorRGBA8* data, int width, int height );
	// Create an image from memory with extra padding between rows.
	// The first pixel of each row is separated by pixels_per_row pixels.
	Image( const ColorRGBA8* data, int width, int height, int pixels_per_row );
	
	// Returns true if the images are the same size and have the same data (for r,g,b,a).
	bool same( const Image& rhs ) const;
	// Returns true if the images are the same size and have the same data for r,g,b (ignoring alpha).
	bool same_rgb( const Image& rhs ) const;
	
	// Saves a PNG image.
	bool save( const std::string& fname ) const;
	bool load( const std::string& fname );
	
	// NOTE: Methods which modify the image return a reference to the image itself so
	//       you can chain methods, as in: img.clear().resize( 100, 100 ).fill( color ).flip();
	
	// Sets the width and height to 0 and data to nullptr.
	Image& clear();
	// Fills the image with the specified color.
	Image& fill( ColorRGBA8 color );
	
	// Access the pixel data starting from the first pixel.
	ColorRGBA8* data();
	const ColorRGBA8* data() const;
	
	// Access the pixel data starting from the first pixel of the given row.
	ColorRGBA8* scanline( int row );
	const ColorRGBA8* scanline( int row ) const;
	
	// Access the pixel data at location x,y
	ColorRGBA8& pixel( int x, int y );
	const ColorRGBA8& pixel( int x, int y ) const;
	
	int width() const;
	int height() const;
	bool empty() const;
	
	// Resizes the image as in a window or canvas resize.
	// Preserves as much as possible the old image (in the upper left).
    // Newly visible pixels are set to `clear`, which defaults to transparent black.
	Image& resize( int width, int height, ColorRGBA8 clear = ColorRGBA8(0,0,0,0) );
	// Resizes the image by scaling it to fit in the new dimensions.
	Image& rescale( int width, int height );
	
	// flip vertically
	Image& flip();
	// flip horizontally
	Image& mirror();
	
protected:
	int m_width = 0;
	int m_height = 0;
	std::vector<ColorRGBA8> m_data; // raw image data
};
}

#endif /* __image_h__ */
