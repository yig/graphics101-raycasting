/*
Author: Yotam Gingold <yotam (strudel) yotamgingold.com>
License: Public Domain [CC0](http://creativecommons.org/publicdomain/zero/1.0/)
GitHub: https://github.com/yig/yimg
*/

#include "image.h"

#include <cassert>
#include <cstring>

#include <string>
#include <algorithm> // copy, swap
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

Image::Image( int width_, int height_ )
    : Image( width_, height_, ColorRGBA8() )
{}

Image::Image( int width_, int height_, ColorRGBA8 color )
    : m_width( width_ ), m_height( height_ ), m_data( width_ * height_, color )
{
    // Dimensions must be non-negative.
    assert( width_ >= 0 && height_ >= 0 );
    // Dimensions must be both 0 or both non-zero.
    assert( width_ > 0 == height_ > 0 );
}

Image::Image( const ColorRGBA8* data, int width_, int height_ )
    : m_width( width_ ), m_height( height_ ), m_data( data, data + width_ * height_ )
{
    // Dimensions must be non-negative.
    assert( width_ >= 0 && height_ >= 0 );
    // Dimensions must be both 0 or both non-zero.
    assert( width_ > 0 == height_ > 0 );
}

Image::Image( const ColorRGBA8* data, int width, int height, int pixels_per_row )
    : Image::Image( width, height )
{
    for( int j = 0; j < height; ++j ) {
        std::copy( data + j*pixels_per_row, data + j*pixels_per_row + width, scanline(j) );
    }
}

ColorRGBA8*       Image::data()       { return m_data.data(); }
const ColorRGBA8* Image::data() const { return m_data.data(); }
ColorRGBA8*       Image::scanline( int row )       { return &m_data[ row*m_width ]; }
const ColorRGBA8* Image::scanline( int row ) const { return &m_data[ row*m_width ]; }

ColorRGBA8& Image::pixel( int x, int y )
{
	assert( x >= 0 && x < m_width );
	assert( y >= 0 && y < m_height );
	
	return m_data[ x + y * m_width ];
}
const ColorRGBA8&
Image::pixel( int x, int y )
const
{
	assert( x >= 0 && x < m_width );
	assert( y >= 0 && y < m_height );
	
	return m_data[ x + y * m_width ];
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
	return m_data.empty();
}

Image& Image::clear()
{
    m_data.clear();
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
    
    // Create a resized image filled with the clear color.
    Image resized( width_, height_, clear_ );
    // If the current image isn't empty, copy what we can.
    if( !empty() )
	{
		const int min_width = std::min( m_width, width_ );
		const int min_height = std::min( m_height, height_ );
		for( int j = 0; j < min_height; ++j ) {
			std::copy( scanline(j), scanline(j) + min_width, resized.scanline(j) );
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
	assert( !m_data.empty() );
	assert( m_width > 0 && m_height > 0 );
	
	// Call stbir_resize_uint8()
	const bool success = stbir_resize_uint8(
	    reinterpret_cast< const unsigned char* >( data() ), width(), height(), 0,
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
	    std::swap( pixel( i,j ), pixel( i, m_height - j - 1 ) );
	}
	
	return *this;
}

Image& Image::mirror()
{
	for( int j = 0; j < m_height; ++j )
	for( int i = 0; i < m_width / 2; ++i )
	{
	    std::swap( pixel( i,j ), pixel( m_width - i - 1, j ) );
	}
	
	return *this;
}

bool Image::same( const Image& rhs ) const
{
    if( m_height != rhs.m_height || m_width != rhs.m_width ) return false;
    
    assert( m_data.size() == m_width*m_height );
    assert( m_data.size() == rhs.m_data.size() );
    
    for( int i = 0; i < m_height*m_width; ++i )
    {
        if(
            m_data[i].r != rhs.m_data[i].r ||
            m_data[i].g != rhs.m_data[i].g ||
            m_data[i].b != rhs.m_data[i].b ||
            m_data[i].a != rhs.m_data[i].a )
            return false;
    }
    
    return true;
}
bool Image::same_rgb( const Image& rhs ) const
{
    if( m_height != rhs.m_height || m_width != rhs.m_width ) return false;
    
    assert( m_data.size() == m_width*m_height );
    assert( m_data.size() == rhs.m_data.size() );
    
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
    assert( !m_data.empty() );
    assert( m_width > 0 && m_height > 0 );
    
    std::string extension = os_path_splitext( fname ).second;
    // ::tolower instead of std::tolower because MingW doesn't find std::tolower.
    // More: https://sourceforge.net/p/mingw/mailman/mingw-users/thread/elbtvatk.fsf%40wanadoo.es/
    std::transform( extension.begin(), extension.end(), extension.begin(), ::tolower );
    
    bool success = false;
    if( extension == ".png" ) {
        success = stbi_write_png( fname.c_str(), m_width, m_height, 4, m_data.data(), 0 );
    } else if( extension == ".bmp" ) {
        success = stbi_write_bmp( fname.c_str(), m_width, m_height, 4, m_data.data() );
    } else if( extension == ".tga" ) {
        success = stbi_write_tga( fname.c_str(), m_width, m_height, 4, m_data.data() );
    } else if( extension == ".jpg" || extension == ".jpeg" ) {
        success = stbi_write_jpg( fname.c_str(), m_width, m_height, 4, m_data.data(), 85 );
    }
    
    return success;
}

bool Image::load( const std::string& fname )
{
    int width_, height_, num_raw_channels;
    unsigned char *data_ = stbi_load( fname.c_str(), &width_, &height_, &num_raw_channels, 4 );
    // An error is indicated by stbi_load() returning NULL.
    if( nullptr == data_ ) {
        stbi_image_free( data_ );
        return false;
    }
    
    // Reallocate memory as needed.
    if( m_width != width_ || m_height != height_ ) {
        m_width = width_;
        m_height = height_;
    }
    // Copy the loaded data.
    // https://stackoverflow.com/questions/2434196/how-to-initialize-stdvector-from-c-style-array
    m_data.assign( reinterpret_cast<ColorRGBA8*>( data_ ), reinterpret_cast<ColorRGBA8*>( data_ ) + m_width * m_height );
    
    stbi_image_free( data_ );
    
    return true;
}

}
