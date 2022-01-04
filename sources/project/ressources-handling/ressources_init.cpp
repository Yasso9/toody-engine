#include "ressources_init.hpp"

#include <cassert>

#include <project/tools/exceptions.hpp>
#include <project/tools/string.hpp>

namespace RessourcesInit
{
    namespace
    {
        // TYPO : peut être réfléchir à mettre ces 2 fonctions static
        std::string get_texture_localisation( TextureKey const & textureKey )
        {
            switch ( textureKey )
            {
            case TextureKey::Tileset :
                return "./ressources/sprites/tileset/ground.png"s;
                break;
            case TextureKey::Player :
                return "./ressources/sprites/players/gold.png"s;
                break;
            case TextureKey::HomeWallpaper :
                return "./ressources/home_wallpaper.jpg"s;
                break;
            default :
                /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
                assert( ( "The texture n"s
                          + std::to_string( static_cast<int>( textureKey ) )
                          + "of the TextureKey class hasn't been loaded"s
                            "or the key doesn't exist in this enum"s )
                            .c_str() );
                return "";
                break;
            }
        }

        std::string get_font_localisation( FontKey const & fontKey )
        {
            switch ( fontKey )
            {
            case FontKey::Arial :
                return "./ressources/arial.ttf"s;
                break;
            default :
                /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
                assert( ( "The texture n"s
                          + std::to_string( static_cast<int>( fontKey ) )
                          + "of the TextureKey class hasn't been loaded"s
                            "or the key doesn't exist in this enum"s )
                            .c_str() );
                return "";
                break;
            }
        }
    } // namespace

    TexturesMap init_textures()
    {
        TexturesMap textures {};

        for ( Enum<TextureKey> textureKey { Enum<TextureKey>::get_min() };
              textureKey < Enum<TextureKey>::get_max();
              ++textureKey )
        {
            sf::Texture texture {};
            std::string const textureLocalisation { get_texture_localisation(
                textureKey.get_value() ) };
            if ( ! texture.loadFromFile( textureLocalisation ) )
            {
                throw FileNotFoundException { textureLocalisation };
            }

            textures.insert(
                std::make_pair( textureKey.get_value(), texture ) );
        }

        return textures;
    }

    FontsMap init_fonts()
    {
        FontsMap fonts {};

        for ( Enum<FontKey> fontKey { Enum<FontKey>::get_min() };
              fontKey < Enum<FontKey>::get_max();
              ++fontKey )
        {
            sf::Font font {};
            std::string const fontLocalisation { get_font_localisation(
                fontKey.get_value() ) };
            if ( ! font.loadFromFile( fontLocalisation ) )
            {
                throw FileNotFoundException { fontLocalisation };
            }

            fonts.insert( std::make_pair( fontKey.get_value(), font ) );
        }

        return fonts;
    }
} // namespace RessourcesInit