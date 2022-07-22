#include "resources.hpp"

#include <filesystem>
#include <map>

#include "tools/exceptions.hpp"
#include "tools/path.hpp"
#include "tools/singleton.hpp"

namespace resources
{
    static bool is_file_suitable(
        std::filesystem::path path,
        std::vector< std::string > imageExtensionHandled );

    sf::Texture const & get_texture( std::string const & file )
    {
        static std::map< std::filesystem::path, sf::Texture > textures {};

        std::filesystem::path const texturePath {
            path::get_folder( path::E_Folder::Resources ) / file
        };
        if ( ! is_file_suitable( texturePath, { ".jpg", ".png" } ) )
        {
            /// @todo mettre une exception plus valable
            throw exception::FileLoadingIssue { texturePath, "Texture" };
        }

        if ( ! textures.contains( texturePath ) )
        { // The texture is new, we load it
            bool textureLoad { true };

            sf::Texture texture {};

            textureLoad &= texture.loadFromFile( texturePath.string() );
            textureLoad &= texture.generateMipmap();

            if ( ! textureLoad )
            {
                throw exception::FileLoadingIssue { texturePath, "Texture" };
            }

            textures.insert( { texturePath, texture } );
        }

        return textures.at( texturePath );
    }

    sf::Font const & get_font( std::string const & file )
    {
        static std::map< std::filesystem::path, sf::Font > fonts {};

        std::filesystem::path const path {
            path::get_folder( path::E_Folder::Resources ) / file
        };
        if ( ! is_file_suitable( path, { ".ttf" } ) )
        {
            /// @todo mettre une excption plus valable
            throw exception::FileLoadingIssue { path, "Font" };
        }

        if ( ! fonts.contains( path ) )
        { // The texture is new, we load it
            sf::Font font {};
            if ( ! font.loadFromFile( path.string() ) )
            {
                throw exception::FileLoadingIssue { path, "Font" };
            }

            /// @todo verify that the texture loaded and the texture insert hhave the same adress
            fonts.insert( { path, font } );
        }

        return fonts.at( path );
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

    sf::Shader & get_shader( std::string const & vertexShaderFile,
                             std::string const & fragmentShaderFile )
    {
        static std::map< S_ShaderFiles, std::unique_ptr< sf::Shader > >
            shaders {};

        S_ShaderFiles shaderFiles {
            path::get_folder( path::E_Folder::Shaders ) / vertexShaderFile,
            path::get_folder( path::E_Folder::Shaders ) / fragmentShaderFile
        };
        if ( ! is_file_suitable( shaderFiles.vertexPath, { ".vert" } ) )
        {
            /// @todo mettre une excption plus valable (spécialisé pour les shaders)
            throw exception::FileLoadingIssue { shaderFiles.vertexPath,
                                                "Shaders" };
        }
        if ( ! is_file_suitable( shaderFiles.fragmentPath, { ".frag" } ) )
        {
            throw exception::FileLoadingIssue { shaderFiles.fragmentPath,
                                                "Shaders" };
        }

        if ( ! shaders.contains( shaderFiles ) )
        { // The texture is new, we load it
            std::unique_ptr< sf::Shader > shader {
                std::make_unique< sf::Shader >()
            };
            if ( ! shader->loadFromFile( shaderFiles.vertexPath.string(),
                                         shaderFiles.fragmentPath.string() ) )
            {
                throw exception::FileLoadingIssue { shaderFiles.vertexPath,
                                                    "Font" };
            }

            shaders.insert( { shaderFiles, std::move( shader ) } );
        }
        return *( shaders.at( shaderFiles ) );
    }

    static bool is_file_suitable(
        std::filesystem::path path,
        std::vector< std::string > imageExtensionHandled )
    {
        // The path should exist and the extension must tell us that is an image
        return std::filesystem::exists( path )
               && std::find( imageExtensionHandled.begin(),
                             imageExtensionHandled.end(),
                             path.extension().string() )
                      != imageExtensionHandled.end();
    }
} // namespace resources