#pragma once

#include <filesystem>
#include <map>

#include "graphics2D/sfml.hpp"
#include "tools/path.hpp"
#include "tools/singleton.hpp"

class Resources final : public Singleton< Resources >
{
    friend Resources & Singleton< Resources >::get_instance();

  public:
    /// @todo remove enum for resources and have a vector or array of dynamique texture and font that can developpe during the utilisation
    enum class E_TextureKey
    {
        Tileset = 0,
        Player,
        HomeWallpaper,
        EnumLast,
    };

    enum class E_FontKey
    {
        Arial = 0,
        EnumLast,
    };

    virtual ~Resources() = default;

    /// @todo deprecated
    sf::Texture const & get_texture( E_TextureKey const & textureKey );
    /// @todo deprecated
    sf::Font const & get_font( E_FontKey const & fontKey );

    sf::Texture const & get_texture( std::string const & imageFile )
    {
        std::filesystem::path texturePath {
            path::get_folder( path::E_Folder::Resources ) / imageFile
        };

        /// @todo verify that the file exist and that it is an image

        if ( ! m_textures.contains( texturePath ) )
        {
            sf::Texture texture {};
            if ( ! texture.loadFromFile( texturePath ) )
            {
                throw std::runtime_error { "Cannot load file : "
                                           + texturePath.string() };
            }

            m_textures.insert( { texturePath, texture } );
        }

        return m_textures.at( texturePath );
    }

  private:
    Resources();

    /// @todo avoir la même map pour font
    std::map< std::filesystem::path, sf::Texture > m_textures;

    /// @todo remove that
    std::map< E_TextureKey, sf::Texture > m_deprecatedTextures;
    std::map< E_FontKey, sf::Font > m_deprecatedFonts;
};