#include "scene.h"

// Some cross-platform path stuff.
// When we can assume C++17, we can use std::filesystem.
#include <QDir>

// textures
#include <QImage>

// Include all of glm here.
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "debugging.h"

#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

using namespace graphics101;

// from_json() needs to be in the namespace of the type.
// However, what graphics101 calls vec3 and what glm calls vec3 differ by float/double.
namespace glm {

void from_json( const json& j, graphics101::vec3& v ) {
    v = graphics101::vec3( j[0], j[1], j[2] );
}
void from_json( const json& j, graphics101::vec4& v ) {
    v = graphics101::vec4( j[0], j[1], j[2], j[3] );
}
void from_json( const json& j, graphics101::mat4& v ) {
    const graphics101::vec4 col0 = j[0];
    const graphics101::vec4 col1 = j[1];
    const graphics101::vec4 col2 = j[2];
    const graphics101::vec4 col3 = j[3];
    v = graphics101::mat4( col0, col1, col2, col3 );
}

}

namespace {

vec3 vec3_from_json( const json& j ) {
    const vec3 result = j;
    return result;
}

}

namespace graphics101 {

void from_json( const json& j, Material& material ) {
    material.color_ambient  = j["color_ambient"];
    material.color_diffuse  = j["color_diffuse"];
    material.color_specular = j["color_specular"];
    material.shininess      = j["shininess"];
    
    material.reflective    = j["reflective"];
    material.color_reflect = j["color_reflect"];
    
    material.refractive          = j["refractive"];
    material.color_refract       = j["color_refract"];
    material.index_of_refraction = j["index_of_refraction"];
    
    if( j.count( "use_diffuse_texture" ) ) {
        material.use_diffuse_texture = j["use_diffuse_texture"];
    }
    if( j.count( "diffuse_texture" ) ) {
        material.diffuse_texture = j["diffuse_texture"];
    }
}
void from_json( const json& j, Light& light ) {
    light.position      = j["position"];
    light.color         = j["color"];
    light.color_ambient = j["color_ambient"];
}

void parse_transform( const json& j, mat4& xform ) {
    // Initialize to the identity.
    xform = mat4(1);
    
    // Parse order (each one right multiplies).
    // 1. Translate
    // 2. Rotate
    // 3. Scale
    // 4. Arbitrary matrix.
    
    // 1. Translate
    if( j.count( "translate" ) ) {
        const vec3 s = j["translate"];
        xform = glm::translate( xform, s );
    }
    // 2. Rotate
    if( j.count( "rotate" ) ) {
        const real degrees = j["rotate"]["degrees"];
        const vec3 axis = j["rotate"]["axis"];
        xform = glm::rotate( xform, glm::radians( degrees ), axis );
    }
    // 3. Scale
    if( j.count( "scale" ) ) {
        const vec3 s = j["scale"];
        xform = glm::scale( xform, s );
    }
    // 4. Arbitrary matrix
    if( j.count( "matrix" ) ) {
        const mat4 m = j["matrix"];
        xform *= m;
    }
}
Scene::ShapePtr parse_shape( const json& j, Scene& scene ) {
    Scene::ShapePtr shape;
    
    std::string type = j["type"];
    if( type == "sphere" ) {
        shape = std::make_shared< Sphere >();
    } else if( type == "plane" ) {
        shape = std::make_shared< Plane >();
    } else if( type == "cylinder" ) {
        shape = std::make_shared< Cylinder >();
    } else if( type == "cone" ) {
        shape = std::make_shared< Cone >();
    } else if( type == "cube" ) {
        shape = std::make_shared< Cube >();
    } else if( type == "mesh" ) {
        Mesh* mesh = new Mesh();
        mesh->loadFromOBJ( j["path"] );
        shape.reset( mesh );
    } else {
        std::cerr << "Unknown shape type: " << type << '\n';
    }
    
    // Return null if we don't have a shape.
    if( !shape ) return shape;
    
    shape->setMaterial( scene.materials[ j["material"] ] );
    shape->setTransform( scene.transforms[ j["transform"] ] );

    return shape;
}

void parse_materials( const json& j, Scene& scene ) {
    scene.materials.clear();
    
    for( const auto& m : j ) {
        scene.materials[ m["name"] ] = m;
    }
}
void parse_lights( const json& j, Scene& scene ) {
    scene.lights.clear();
    
    scene.lights.resize( j.size() );
    for( int i = 0; i < j.size(); ++i ) {
        scene.lights[i] = j[i];
    }
}
void parse_transforms( const json& j, Scene& scene ) {
    scene.transforms.clear();
    
    for( const auto& m : j ) {
        parse_transform( m, scene.transforms[ m["name"] ] );
    }
}

void parse_camera( const json& j, Scene& scene ) {
    if( j["type"] == "perspective" ) {
        scene.camera = std::make_shared< CameraPerspective >( j["focal_length"].get<real>() );
    } else {
        scene.camera = std::make_shared< CameraOrthographic >();
    }
    
    scene.camera->setEye( vec3_from_json( j["eye"] ) );
    scene.camera->setFrameFromLookUp( vec3_from_json( j["look"] ), vec3_from_json( j["up"] ) );
    scene.camera->setFilm(
        j["film_left"].get<real>(),
        j["film_right"].get<real>(),
        j["film_bottom"].get<real>(),
        j["film_top"].get<real>()
        );
}

void parse_shapes( const json& j, Scene& scene ) {
    scene.shapes.clear();
    
    scene.shapes.resize( j.size() );
    for( int i = 0; i < j.size(); ++i ) {
        scene.shapes[i] = parse_shape( j[i], scene );
    }
}

void parse_textures( const json& j, Scene& scene ) {
    // Iterate over object properties
    for( auto it = j.begin(); it != j.end(); ++it ) {
        QImage image;
        const std::string image_path = it.value().get<std::string>();
        if( !image.load( image_path.c_str() ) ) {
            std::cerr << "Error loading image: " << image_path << '\n';
            // We'll still store this QImage object in the dictionary of
            // textures so that we can "fail gracefully" and let the image
            // render with undefined.
        }
        
        // Ensure the image is ARGB format.
        // TODO Q: What happens if the format is `Format_Invalid` due to a failed load?
        if( image.format() != QImage::Format_ARGB32 ) image = image.convertToFormat( QImage::Format_ARGB32 );
        
        // Store the image.
        scene.textures[it.key()] = image;
    }
}

void from_json( const json& j, Scene& scene ) {
    parse_camera( j["camera"], scene );
    parse_materials( j["materials"], scene );
    parse_lights( j["lights"], scene );
    parse_transforms( j["transforms"], scene );
    // Parse shapes last, because they reference materials and transforms.
    parse_shapes( j["shapes"], scene );
    
    // Optionally parse textures.
    scene.textures.clear();
    if( j.count( "textures" ) ) {
        parse_textures( j["textures"], scene );
    }
}

// Push current working directory. RAII.
namespace {
class PushDir {
public:
    PushDir( const std::string& path ) {
        // Save the current working directory.
        m_oldPath = QDir::currentPath();
        // Set the current working directory to what the user asked.
        QDir::setCurrent( QString( path.c_str() ) );
    }
    ~PushDir() {
        // Restore the current working directory.
        QDir::setCurrent( m_oldPath );
    }
private:
    QString m_oldPath;
};
}

bool Scene::parse( const std::string& path_to_file ) {
    std::ifstream infile( path_to_file );
    if( !infile ) return false;
    json j;
    infile >> j;
    // There may be relative paths in the JSON. Temporarily set the current working
    // directory to the directory of the scene file.
    {
        PushDir( QDir::toNativeSeparators( QFileInfo( QString( path_to_file.c_str() ) ).dir().path() ).toStdString() );
        *this = j;
    }
    return true;
    
    /*
    // A hard-coded scene.
    camera = std::make_shared< CameraPerspective >( 1 );

    // A sphere
    ShapePtr s = std::make_shared< Sphere >();
    // Make the sphere's center at (0,0,-4).
    s->setTransform( glm::translate( mat4(1), vec3( 0, 0, -4 ) ) );
    // Make the sphere green.
    s->material().color_diffuse = vec3( 0, 1, 0 );
    s->material().reflective = true;
    s->material().color_reflect = vec3( .1, .1, .1 );
    shapes.push_back( s );

    // A cylinder
    s = std::make_shared< Cylinder >();
    // Make the cylinder's center at (1,0,-4).
    s->setTransform( glm::rotate(
                         glm::translate( mat4(1), vec3( 2, 1, -4 ) ),
                         glm::radians(-45.), camera->right() ) );
    // Make the cylinder purple.
    s->material().color_diffuse = vec3( 1, 0, 1 );
    s->material().reflective = true;
    s->material().color_reflect = vec3( .1, .1, .1 );
    shapes.push_back( s );

    // A plane
    s = std::make_shared< Plane >();
    // Rotate the plane so that it is parallel to the xz axis.
    // So that the normal points up,
    // we want to rotate around the x-axis 90 degrees.
    // We then translate so that the plane passes through (0,-1,0).
    s->setTransform( glm::rotate(
                         glm::translate( mat4(1), vec3( 0, -1, 0 ) ),
                         glm::radians(-90.), camera->right() ) );
    // Make the plane blue.
    s->material().color_diffuse = vec3( 0, 0, 1 );
    s->material().reflective = true;
    s->material().color_reflect = vec3( .1, .1, .1 );
    shapes.push_back( s );

    Light l;
    // Headlight
    // l.position = camera->eye();
    // Above the eye.
    l.position = vec3( 0., 3., 0 );
    lights.push_back( l );

    return true;
    */
}

}
