/*
Author: Yotam Gingold <yotam (strudel) yotamgingold.com>
License: Public Domain [CC0](http://creativecommons.org/publicdomain/zero/1.0/)
GitHub: https://github.com/yig/yimg
*/

#include "image.h"

#include <cassert>
#include <cstring>
#include <stdlib.h> // malloc, free, realloc

#include <string>
#include <algorithm> // copy
#include <cctype> // tolower
#include <utility> // move


// For load()
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// For save()
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// For rescale()
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace
{
/// From my: stl.h

// Behaves like the python os.path.splitext() function.
inline std::pair< std::string, std::string > os_path_splitext( const std::string& path )
{
    const std::string::size_type split_dot = path.find_last_of( "." );
    const std::string::size_type split_slash = path.find_last_of( "/" );
    if( split_dot != std::string::npos && (split_slash == std::string::npos || split_slash < split_dot) )
        return std::make_pair( path.substr( 0, split_dot ), path.substr( split_dot ) );
    else
        return std::make_pair( path, std::string() );
}
}

namespace graphics101
{
static_assert( sizeof( ColorRGBA8 ) == 4, "Color struct shouldn't be padded" );

Image::Image() {}

Image::Image( const Image& rhs )
{
	*this = rhs;
}

Image::Image( Image&& rhs )
{
    *this = std::move( rhs );
}

Image::Image( int width_, int height_ )
    : m_width( width_ ), m_height( height_ )
{
    // Dimensions must be non-negative.
    assert( width_ >= 0 && height_ >= 0 );
    // Dimensions must be both 0 or both non-zero.
    assert( width_ > 0 == height_ > 0 );
    
    // Stop with m_data = nullptr (set by the default initializer in the header).
    if( width_ == 0 || height_ == 0 ) return;
    
    m_data = reinterpret_cast<ColorRGBA8*>( malloc( width_ * height_ * sizeof(ColorRGBA8) ) );
}

Image::Image( const ColorRGBA8* data, int width, int height )
    : Image::Image( width, height )
{
    memcpy( m_data, data, width * height * sizeof(ColorRGBA8) );
}

Image::Image( const ColorRGBA8* data, int width, int height, int pixels_per_row )
    : Image::Image( width, height )
{
    for( int j = 0; j < height; ++j ) {
        memcpy( scanline(j), data + j*pixels_per_row, width * sizeof(ColorRGBA8) );
    }
}

Image&
Image::operator=( const Image& rhs )
{
    if( !rhs.m_data )
    {
        assert( 0 == rhs.m_width );
        assert( 0 == rhs.m_height );
        
        clear();
        
        return *this;
    }
    
	if( m_width != rhs.m_width || m_height != rhs.m_height )
	{
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		m_data = reinterpret_cast< ColorRGBA8* >( realloc( m_data, m_width * m_height * sizeof(ColorRGBA8) ) );
	}
	
	assert( m_data );
	memcpy( m_data, rhs.m_data, m_width * m_height * sizeof(ColorRGBA8) );
	
	return *this;
}
Image&
Image::operator=( Image&& rhs )
{
    // The move operator takes the data from rhs, so long as they aren't the same object.
    if( this != &rhs ) {
        // Erase the current image.
        clear();
        
        // Take data from `rhs`.
        m_width = rhs.m_width;
        m_height = rhs.m_height;
        m_data = rhs.m_data;
        
        // Set `rhs` to a valid state without its data.
        rhs.m_width = rhs.m_height = 0;
        rhs.m_data = nullptr;
    }
    return *this;
}

Image::~Image() { clear(); }

ColorRGBA8*       Image::data()       { return m_data; }
const ColorRGBA8* Image::data() const { return m_data; }
ColorRGBA8*       Image::scanline( int row )       { return m_data + row*m_width; }
const ColorRGBA8* Image::scanline( int row ) const { return m_data + row*m_width; }

ColorRGBA8& Image::pixel( int i, int j )
{
	assert( i >= 0 && i < m_width );
	assert( j >= 0 && j < m_height );
	
	return m_data[ i + j * m_width ];
}
const ColorRGBA8&
Image::pixel( int i, int j )
const
{
	assert( i >= 0 && i < m_width );
	assert( j >= 0 && j < m_height );
	
	return m_data[ i + j * m_width ];
}

int Image::width() const
{
	return m_width;
}
int Image::height() const
{
	return m_height;
}
bool Image::empty() const
{
	return m_data == nullptr;
}

Image& Image::clear()
{
    free( m_data );
    
    m_data = nullptr;
    m_width = m_height = 0;
    
    return *this;
}

Image& Image::fill( ColorRGBA8 color )
{
    if( empty() ) return *this;
    
    for( int i = 0; i < m_width*m_height; ++i ) m_data[i] = color;
    
    return *this;
}

Image& Image::resize( int width_, int height_, ColorRGBA8 clear_ )
{
    // If new dimensions are the same, do nothing.
	if( m_width == width_ && m_height == height_ ) return *this;
    
    // Create a resized image.
    Image resized( width_, height_ );
    // Fill it with the clear color.
    resized.fill( clear_ );
    // If the current image isn't empty, copy what we can.
    if( m_data )
	{
		const int min_width = std::min( m_width, width_ );
		const int min_height = std::min( m_height, height_ );
		for( int j = 0; j < min_height; ++j ) {
			memcpy( resized.scanline(j), scanline(j), min_width * sizeof(ColorRGBA8) );
		}
	}
	// Set the current image to the new resized image (by moving its data over).
	*this = std::move(resized);
	return *this;
}


Image& Image::rescale( int width_, int height_ )
{
    // If new dimensions are the same, do nothing.
	if( m_width == width_ && m_height == height_ ) return *this;
    
    Image rescaled( width_, height_ );
    
    // If there is currently no data, set the output to transparent black and return.
    if( empty() ) {
        rescaled.fill( ColorRGBA8(0,0,0,0) );
        // Set the current image to the new image (by moving its data over).
        *this = std::move(rescaled);
        return *this;
    }
    
	// If we are here, we have data.
	assert( m_data );
	assert( m_width > 0 && m_height > 0 );
	
	// Call stbir_resize_uint8()
	const bool success = stbir_resize_uint8(
	    reinterpret_cast< const unsigned char* >( m_data ), m_width, m_height, 0,
	    reinterpret_cast< unsigned char* >( rescaled.data() ), width_, height_, 0,
	    4 // number of channels
	    );
	assert( success );
	
	// Set the current image to the new image (by moving its data over).
    *this = std::move(rescaled);
    return *this;
}

Image& Image::flip()
{
	for( int j = 0; j < m_height / 2; ++j )
	for( int i = 0; i < m_width; ++i )
	{
		ColorRGBA8* lhs = &pixel( i,j );
		ColorRGBA8* rhs = &pixel( i, m_height - j - 1 );
		
		ColorRGBA8 temp = *lhs;
		
		*lhs = *rhs;
		*rhs = temp;
	}
	
	return *this;
}

Image& Image::mirror()
{
	for( int j = 0; j < m_height; ++j )
	for( int i = 0; i < m_width / 2; ++i )
	{
		ColorRGBA8* lhs = &pixel( i,j );
		ColorRGBA8* rhs = &pixel( m_width - i - 1, j );
		
		ColorRGBA8 temp = *lhs;
		
		*lhs = *rhs;
		*rhs = temp;
	}
	
	return *this;
}

bool Image::same( const Image& rhs ) const
{
    if( m_height != rhs.m_height || m_width != rhs.m_width ) return false;
    
    // Check if this->m_data and rhs.m_data point to the same data.
    // This also checks for if both are nullptr.
    if( m_data == rhs.m_data ) return true;
    
    assert( m_data && rhs.m_data );
    return 0 == memcmp( m_data, rhs.m_data, m_width * m_height * sizeof(ColorRGBA8) );
}
bool Image::same_rgb( const Image& rhs ) const
{
    if( m_height != rhs.m_height || m_width != rhs.m_width ) return false;
    
    // Check if this->m_data and rhs.m_data point to the same data.
    // This also checks for if both are nullptr.
    if( m_data == rhs.m_data ) return true;
    
    assert( m_data && rhs.m_data );
    
    for( int i = 0; i < m_height*m_width; ++i )
    {
        if(
            m_data[i].r != rhs.m_data[i].r ||
            m_data[i].g != rhs.m_data[i].g ||
            m_data[i].b != rhs.m_data[i].b )
            return false;
    }
    
    return true;
}

bool Image::save( const std::string& fname )
const
{
    // Don't call this with an empty image.
    assert( m_data );
    assert( m_width > 0 && m_height > 0 );
    
    std::string extension = os_path_splitext( fname ).second;
    std::transform( extension.begin(), extension.end(), extension.begin(), std::tolower );
    
    bool success = false;
    if( extension == ".png" ) {
        success = stbi_write_png( fname.c_str(), m_width, m_height, 4, m_data, 0 );
    } else if( extension == ".bmp" ) {
        success = stbi_write_bmp( fname.c_str(), m_width, m_height, 4, m_data );
    } else if( extension == ".tga" ) {
        success = stbi_write_tga( fname.c_str(), m_width, m_height, 4, m_data );
    } else if( extension == ".jpg" || extension == ".jpeg" ) {
        success = stbi_write_jpg( fname.c_str(), m_width, m_height, 4, m_data, 85 );
    }
    
    return success;
}

bool Image::load( const std::string& fname )
{
    int width_, height_, num_raw_channels;
    unsigned char *data = stbi_load( fname.c_str(), &width_, &height_, &num_raw_channels, 4 );
    // An error is indicated by stbi_load() returning NULL.
    if( nullptr == data ) {
        stbi_image_free( data );
        return false;
    }
    
    // Reallocate memory as needed.
    if( m_width != width_ || m_height != height_ ) {
        m_width = width_;
        m_height = height_;
        m_data = reinterpret_cast<ColorRGBA8*>( realloc( m_data, m_width * m_height * sizeof(ColorRGBA8) ) );
    }
    assert( m_data );
    
    // Copy the loaded data.
    memcpy( m_data, data, m_width * m_height * sizeof(ColorRGBA8) );
    
    stbi_image_free( data );
    
    return true;
}

}
