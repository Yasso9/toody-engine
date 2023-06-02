#include "resources.hpp"

#include <algorithm>   // for find
#include <compare>     // for operator<, strong_ordering
#include <filesystem>  // for path, operator/, operator<=>
#include <iostream>
#include <map>      // for map
#include <memory>   // for unique_ptr, make_unique
#include <utility>  // for move
#include <vector>   // for vector

#include <SFML/Graphics/Font.hpp>     // for Font
#include <SFML/Graphics/Shader.hpp>   // for Shader
#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "tools/path.hpp"  // for get_folder, E_Folder, E_Folder:...
#include "tools/traces.hpp"

namespace resources
{
    static bool is_file_suitable (
        std::filesystem::path      path,
        std::vector< std::string > imageExtensionHandled );

    sf::Texture const & get_texture ( std::filesystem::path const & file )
    {
        static std::map< std::filesystem::path, sf::Texture > textures {};

        if ( ! is_file_suitable( file, { ".jpg", ".png" } ) )
        {
            std::cerr << "File : " << file << " isn't suitable for texture"
                      << std::endl;
        }

        if ( ! textures.contains( file ) )
        {  // The texture is new, we load it
            bool textureLoad { true };

            sf::Texture texture {};

            textureLoad &= texture.loadFromFile( file.string() );
            textureLoad &= texture.generateMipmap();

            if ( ! textureLoad )
            {
                std::cerr << "Impossible to load file " << file
                          << " for texture";
            }

            textures.insert( { file, texture } );
        }

        return textures.at( file );
    }

    sf::Font const & get_font ( std::filesystem::path const & file )
    {
        static std::map< std::filesystem::path, sf::Font > fonts {};

        if ( ! is_file_suitable( file, { ".ttf" } ) )
        {
            std::cerr << "File : " << file << " isn't suitable for font"
                      << std::endl;
        }

        if ( ! fonts.contains( file ) )
        {  // The texture is new, we load it
            sf::Font font {};
            if ( ! font.loadFromFile( file.string() ) )
            {
                std::cerr << "Impossible to load file " << file << " for font";
            }

            fonts.insert( { file, font } );
        }

        return fonts.at( file );
    }

    struct S_ShaderFiles
    {
        std::filesystem::path vertexPath {};
        std::filesystem::path fragmentPath {};

        bool operator<( S_ShaderFiles const & rhs ) const
        {
            return this->vertexPath < rhs.vertexPath
                   && this->fragmentPath < rhs.fragmentPath;
        }
    };

    sf::Shader & get_shader ( std::string const & vertexShaderFile,
                              std::string const & fragmentShaderFile )
    {
        static std::map< S_ShaderFiles, std::unique_ptr< sf::Shader > >
            shaders {};

        S_ShaderFiles shaderFiles {
            path::get_folder( path::E_Folder::Shaders ) / vertexShaderFile,
            path::get_folder( path::E_Folder::Shaders ) / fragmentShaderFile };
        if ( ! is_file_suitable( shaderFiles.vertexPath, { ".vert" } ) )
        {
            Trace::FileIssue( shaderFiles.vertexPath, "Shaders extension" );
        };
        if ( ! is_file_suitable( shaderFiles.fragmentPath, { ".frag" } ) )
        {
            Trace::FileIssue( shaderFiles.vertexPath, "Shaders extension" );
        }

        if ( ! shaders.contains( shaderFiles ) )
        {  // The texture is new, we load it
            std::unique_ptr< sf::Shader > shader {
                std::make_unique< sf::Shader >() };
            if ( ! shader->loadFromFile( shaderFiles.vertexPath.string(),
                                         shaderFiles.fragmentPath.string() ) )
            {
                Trace::FileIssue( shaderFiles.vertexPath, "Shaders" );
                Trace::FileIssue( shaderFiles.fragmentPath, "Shaders" );
            }

            shaders.insert( { shaderFiles, std::move( shader ) } );
        }
        return *( shaders.at( shaderFiles ) );
    }

    static bool is_file_suitable (
        std::filesystem::path      path,
        std::vector< std::string > imageExtensionHandled )
    {
        // The path should exist and the extension must tell us that is an image
        return std::filesystem::exists( path )
               && std::find( imageExtensionHandled.begin(),
                             imageExtensionHandled.end(),
                             path.extension().string() )
                      != imageExtensionHandled.end();
    }
}  // namespace resources
