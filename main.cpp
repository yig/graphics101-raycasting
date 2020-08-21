#include "scene.h"

#include <iostream> // std::cout and std::cerr
#include <cstdio> // atoi()
#include <cassert> // assert()

using namespace graphics101;

void usage( const char* argv0 ) {
    std::cerr << "Usage: " << argv0 << " path/to/scene.json image_out.png long_edge_pixels\n";
    std::exit(-1);
}

int main( int argc, char* argv[] ) {
    if( argc != 4 ) {
        usage( argv[0] );
    }
    
    const std::string inpath( argv[1] );
    const std::string outpath( argv[2] );
    const int long_edge_pixels( atoi( argv[3] ) );
    if( long_edge_pixels <= 0 ) {
        std::cerr << "Long edge pixels must be a positive integer: " << long_edge_pixels << std::endl;
        usage( argv[0] );
    }
    
    Scene scene;
    const bool success = scene.parse( argv[1] );
    if( !success ) {
        std::cerr << "Error parsing: " << inpath << std::endl;
        usage( argv[0] );
    }
    
    int width, height;
    scene.camera->imageDimensionsWithLongEdge( long_edge_pixels, width, height );
    assert( width > 0 );
    assert( height > 0 );
    Image image = scene.render( width, height );
    
    image.save( outpath );
    std::cout << "Saved: " << outpath << std::endl;
    
    return 0;
}
