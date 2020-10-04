#include "shape.h"

// mesh parsing
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace {
// Helper functions

struct VertexBundle {
    VertexBundle() {}
    VertexBundle( const std::string& str ) { fromString( str ); }
    void fromString( const std::string& str ) {
        std::string v_str, vt_str, vn_str;

        // Grab everything up to the first '/'
        auto first_slash = str.find( '/' );
        v_str = str.substr( 0, first_slash );

        if( first_slash != std::string::npos ) {
            auto second_slash = str.find( '/', first_slash+1 );
            if( second_slash == std::string::npos ) {
                vt_str = str.substr( first_slash+1 );
            } else {
                vt_str = str.substr( first_slash+1, second_slash-(first_slash+1) );
                vn_str = str.substr( second_slash+1 );
            }
        }

        if( !v_str.empty()  ) v  = std::stoi(v_str);
        if( !vt_str.empty() ) vt = std::stoi(vt_str);
        if( !vn_str.empty() ) vn = std::stoi(vn_str);
    }

    int v = 0;
    int vt = 0;
    int vn = 0;
};

}

namespace graphics101 {

// Wikipedia has a nice definition of the Wavefront OBJ file format:
//    https://en.wikipedia.org/wiki/Wavefront_.obj_file
bool Mesh::loadFromOBJ( const std::string& path ) {
    using namespace std;
    
    // Clear the mesh.
    {
        positions.clear();
        face_positions.clear();
        
        normals.clear();
        face_normals.clear();
        
        texcoords.clear();
        face_texcoords.clear();
    }
    
    // TODO: Error checking with a printout to std::cerr and return false.
    
    // Open the file.
    ifstream mesh( path );
    if( !mesh ) {
        cerr << "ERROR: Unable to access path: " << path << '\n';
        return false;
    }
    while( !( mesh >> ws ).eof() ) {
        // Get each line.
        string line;
        getline( mesh, line );
        
        // Skip blank lines.
        if( line.empty() ) continue;
        
        istringstream linestream( line );
        
        // Get the first word.
        string first_word;
        linestream >> first_word;
        
        if( first_word == "v" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            positions.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vn" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            normals.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vt" ) {
            real x,y;
            linestream >> x >> y;
            texcoords.push_back( vec2( x,y ) );
        }
        else if( first_word == "f" ) {
            std::vector< string > fb;
            fb.reserve( 4 );
            while( !( linestream >> ws ).eof() ) {
                string b;
                linestream >> b;
                fb.push_back( b );
            }
            if( fb.size() < 3 ) {
                cerr << "ERROR: Skipping a face with less than 3 vertices.\n";
                continue;
            }
            
            // OBJ files are 1-indexed
            // Negative indices subtract.
            
            std::vector< VertexBundle > vb;
            vb.resize( fb.size() );
            for( int i = 0; i < vb.size(); ++i ) {
                // Convert the string separated by slashes to integers.
                vb[i] = VertexBundle( fb[i] );
                
                // We must have positions, so they can't be zero.
                assert( vb[i].v != 0 );
                
                // If any index is negative, add it to that attribute's length.
                // Otherwise, subtract 1.
                if( vb[i].v  < 0 ) vb[i].v  += positions.size(); else vb[i].v  -= 1;
                if( vb[i].vt < 0 ) vb[i].vt += texcoords.size(); else vb[i].vt -= 1;
                if( vb[i].vn < 0 ) vb[i].vn += normals.size();   else vb[i].vn -= 1;
            }
            
            // We either have no normals/texcoords or we have normal/texcoord faces
            // in correspondence with position faces.
            assert( face_normals.size() == 0 || face_normals.size() == face_positions.size() );
            assert( face_texcoords.size() == 0 || face_texcoords.size() == face_positions.size() );
            
            // Add all the faces.
            if( vb.size() != 3 ) {
                cerr << "Triangulating a face with " << vb.size() << " vertices.\n";
            }
            for( int i = 2; i < vb.size(); ++i ) {
                // Add the position face.
                face_positions.emplace_back( Triangle{ vb[0].v, vb[i-1].v, vb[i].v } );
                
                // If one vertex bundle has normals or texcoords,
                // they all must have normals/texcoords.
                // We have already converted the OBJ coordinates back to 0-indexing,
                // so check for -1.
                assert( ( vb[0].vn == -1 ) == ( vb[i-1].vn == -1 ) );
                assert( ( vb[0].vn == -1 ) == ( vb[i].vn == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i-1].vt == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i].vt == -1 ) );
                
                if( vb[0].vn != -1 ) {
                    face_normals.emplace_back( Triangle{ vb[0].vn, vb[i-1].vn, vb[i].vn } );
                }
                
                if( vb[0].vt != -1 ) {
                    face_texcoords.emplace_back( Triangle{ vb[0].vt, vb[i-1].vt, vb[i].vt } );
                }
            }
        }
    }
    
    return true;
}

}
