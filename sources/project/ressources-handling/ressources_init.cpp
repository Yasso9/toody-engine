#include "ressources_init.hpp"

namespace RessourcesInit
{
    namespace
    {
        std::string get_texture_localisation( TextureKey const & textureKey )
        {
            switch ( textureKey )
            {
            case TextureKey::Tileset :
                // Credit GragarLink10
                return "./ressources/sprites/players/gold.png";
                break;
            case TextureKey::Player :
                return "./ressources/sprites/players/gold.png";
                break;
            case TextureKey::HomeWallpaper :
                return "./ressources/sprites/players/gold.png";
                break;
            default :
                /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
                throw std::runtime_error(
                    "The texture n" + std::to_string( static_cast<int>( textureKey ) )
                    + "of the TextureKey class hasn't been loaded"
                      "or the key doesn't exist in this enum" );
                break;
            }
        }
    } // namespace

    std::map<TextureKey, sf::Texture> init_textures()
    {
        assert( TextureKey::Tileset == static_cast<TextureKey>( 0 )
                && ( "Tileset isn't the first key of TextureKey,"
                     "this can create a dangerous load of ressources" ) );

        std::map<TextureKey, sf::Texture> textures {};

        for ( int textureKey { static_cast<int>( TextureKey::Tileset ) };
              textureKey < static_cast<int>( TextureKey::Count );
              ++textureKey )
        {
            sf::Texture texture {};
            std::string const textureLocalisation { get_texture_localisation(
                static_cast<TextureKey>( textureKey ) ) };
            if ( ! texture.loadFromFile( textureLocalisation ) )
            {
                throw std::runtime_error( "There isn't a file named \""
                                          + textureLocalisation + "\"" );
            }

            textures.insert(
                std::make_pair( static_cast<TextureKey>( textureKey ), texture ) );
        }

        return textures;
    }

    // TYPO change this function to make it look like init_textures
    std::map<FontKey, sf::Font> init_fonts()
    {
        sf::Font fontArial {};
        if ( ! fontArial.loadFromFile( "./ressources/arial.ttf" ) )
        {
            throw std::runtime_error(
                "There isn't a file named \"Police/arial.ttf\"" );
        }

        return {
            { FontKey::Arial, fontArial },
        };
    }
} // namespace RessourcesInit