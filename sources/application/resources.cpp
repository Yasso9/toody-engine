#include "resources.hpp"

#include <algorithm>                  // for find
#include <compare>                    // for operator<, strong_ordering
#include <filesystem>                 // for path, operator/, operator<=>
#include <iostream>
#include <map>                        // for map
#include <memory>                     // for unique_ptr, make_unique
#include <utility>                    // for move
#include <vector>                     // for vector

#include <SFML/Graphics/Font.hpp>     // for Font
#include <SFML/Graphics/Shader.hpp>   // for Shader
#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "tools/traces.hpp"

namespace
{
    bool is_file_suitable ( fspath                     path,
                            std::vector< std::string > imageExtensionHandled )
    {
        // The path should exist and the extension must tell us that is an image
        return std::filesystem::exists( path )
               && std::find( imageExtensionHandled.begin(),
                             imageExtensionHandled.end(),
                             path.extension().string() )
                      != imageExtensionHandled.end();
    }

    sf::Texture const & get_texture ( fspath const & file )
    {
        static std::map< fspath, sf::Texture > textures {};

        if ( ! is_file_suitable( file, { ".jpg", ".png" } ) )
        {
            Trace::Warning( "{} isn't suitable for texture", file.string() );
        }

        if ( ! textures.contains( file ) )
        {  // The texture is new, we load it
            sf::Texture texture {};

            if ( ! texture.loadFromFile( file.string() ) )
            {
                Trace::Error( "Impossible to load {} for texture",
                              file.string() );
            }
            if ( ! texture.generateMipmap() )
            {
                Trace::Error( "Impossible to generate mipmap for {}",
                              file.string() );
            }

            textures.insert( { file, texture } );
        }

        return textures.at( file );
    }

    sf::Font const & get_font ( fspath const & file )
    {
        static std::map< fspath, sf::Font > fonts {};

        if ( ! is_file_suitable( file, { ".ttf" } ) )
        {
            Trace::Warning( "{} isn't suitable for font", file.string() );
        }

        if ( ! fonts.contains( file ) )
        {  // The texture is new, we load it
            sf::Font font {};

            if ( ! font.loadFromFile( file.string() ) )
            {
                Trace::Error( "Impossible to load {} for font", file.string() );
            }

            fonts.insert( { file, font } );
        }

        return fonts.at( file );
    }

    struct S_ShaderFiles
    {
        fspath vertexPath {};
        fspath fragmentPath {};

        // We need to define the operator< to use the map
        bool operator< ( S_ShaderFiles const & rhs ) const
        {
            return this->vertexPath.string() < rhs.vertexPath.string()
                   && this->fragmentPath.string() < rhs.fragmentPath.string();
        }
    };

    sf::Shader & get_shader ( fspath const & vertexShaderFile,
                              fspath const & fragmentShaderFile )
    {
        static std::map< S_ShaderFiles, std::unique_ptr< sf::Shader > >
            shaders {};

        S_ShaderFiles shaderFiles { vertexShaderFile, fragmentShaderFile };
        if ( ! is_file_suitable( shaderFiles.vertexPath, { ".vert" } ) )
        {
            Trace::FileIssue( shaderFiles.vertexPath,
                              "Vertex Shader extension" );
        };
        if ( ! is_file_suitable( shaderFiles.fragmentPath, { ".frag" } ) )
        {
            Trace::FileIssue( shaderFiles.fragmentPath,
                              "Fragment Shader extension" );
        }

        if ( ! shaders.contains( shaderFiles ) )
        {  // The texture is new, we load it
            std::unique_ptr< sf::Shader > shader {
                std::make_unique< sf::Shader >() };
            if ( ! shader->loadFromFile( shaderFiles.vertexPath.string(),
                                         shaderFiles.fragmentPath.string() ) )
            {
                Trace::Error( "Impossible to load {} and {} for shader",
                              shaderFiles.vertexPath.string(),
                              shaderFiles.fragmentPath.string() );
            }

            shaders.insert( { shaderFiles, std::move( shader ) } );
        }
        return *( shaders.at( shaderFiles ) );
    }
}  // namespace

namespace resource
{
    namespace app_data
    {
        fspath get_path ( std::string const & name )
        {
            return app_data::FOLDER / name;
        }
    }  // namespace app_data

    namespace font
    {
        fspath get_path ( std::string const & name )
        {
            return font::FOLDER / name;
        }

        sf::Font const & get ( std::string const & name )
        {
            return get_font( font::get_path( name ) );
        }
    }  // namespace font

    namespace tileset
    {
        fspath get_path ( std::string const & name )
        {
            return tileset::FOLDER / name;
        }

        sf::Texture const & get ( std::string const & name )
        {
            return get_texture( tileset::get_path( name ) );
        }
    }  // namespace tileset

    namespace example
    {
        fspath get_path ( std::string const & name )
        {
            return example::FOLDER / name;
        }
    }  // namespace example

    namespace object3D
    {
        fspath get_path ( std::string const & name )
        {
            return object3D::FOLDER / name;
        }
    }  // namespace object3D

    namespace shader
    {
        fspath get_path ( std::string const & name )
        {
            return shader::FOLDER / name;
        }

        sf::Shader & get ( std::string const & vertexName,
                           std::string const & fragmentName )
        {
            return get_shader( shader::get_path( vertexName ),
                               shader::get_path( fragmentName ) );
        }
    }  // namespace shader

    namespace image
    {
        fspath get_path ( std::string const & name )
        {
            return image::FOLDER / name;
        }

        sf::Texture const & get ( std::string const & name )
        {
            return get_texture( image::get_path( name ) );
        }
    }  // namespace image

    namespace character
    {
        fspath get_path ( std::string const & name )
        {
            return character::FOLDER / name;
        }

        sf::Texture const & get ( std::string const & name )
        {
            return get_texture( character::get_path( name ) );
        }
    }  // namespace character
}  // namespace resource
