#include "ressources_init.hpp"

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/exceptions.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

static std::string get_texture_localisation( E_TextureKey const & textureKey );
static std::string get_font_localisation( E_FontKey const & fontKey );

// TYPO rename this namespace
namespace RessourcesInit
{
    T_TexturesMap init_textures()
    {
        T_TexturesMap textures {};

        for ( Enum<E_TextureKey> textureKey { Enum<E_TextureKey>::get_min() };
              textureKey < Enum<E_TextureKey>::get_max();
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

    T_FontsMap init_fonts()
    {
        T_FontsMap fonts {};

        for ( Enum<E_FontKey> fontKey { Enum<E_FontKey>::get_min() };
              fontKey < Enum<E_FontKey>::get_max();
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

static std::string get_texture_localisation( E_TextureKey const & textureKey )
{
    std::string imagePath { tools::get_path::resources() };

    switch ( textureKey )
    {
    case E_TextureKey::Tileset :
        imagePath += "/sprites/tileset/ground.png"s;
        break;
    case E_TextureKey::Player :
        imagePath += "/gold_sprite.png"s;
        break;
    case E_TextureKey::HomeWallpaper :
        imagePath += "/home_wallpaper.jpg"s;
        break;
    default :
        /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
        ASSERTION( false,
                   "A texture of the E_TextureKey class hasn't been loaded"s
                   "or the key doesn't exist in this enum"s );
        break;
    }

    return imagePath;
}

static std::string get_font_localisation( E_FontKey const & fontKey )
{
    std::string fontPath { tools::get_path::resources() };

    switch ( fontKey )
    {
    case E_FontKey::Arial :
        fontPath += "/arial.ttf"s;
        break;
    default :
        ASSERTION( false,
                   "A texture of the E_TextureKey class hasn't been loaded"s
                   "or the key doesn't exist in this enum"s );
        break;
    }

    return fontPath;
}