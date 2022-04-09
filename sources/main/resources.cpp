#include "resources.hpp"

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/exceptions.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

static void init_textures(
    std::map< Resources::E_TextureKey, sf::Texture > & textures );
static void init_fonts(
    std::map< Resources::Resources::E_FontKey, sf::Font > & fonts );
static std::string get_texture_localisation(
    Resources::E_TextureKey const & textureKey );
static std::string get_font_localisation(
    Resources::E_FontKey const & fontKey );

Resources::Resources()
{
    init_textures( this->m_textures );
    init_fonts( this->m_fonts );
}

sf::Texture const & Resources::get_texture(
    Resources::E_TextureKey const & textureKey )
{
    return this->m_textures.at( textureKey );
}
sf::Font const & Resources::get_font( Resources::E_FontKey const & fontKey )
{
    return this->m_fonts.at( fontKey );
}

static void init_textures(
    std::map< Resources::E_TextureKey, sf::Texture > & textures )
{
    textures.clear();

    for ( Enum< Resources::E_TextureKey > textureKey {
              Enum< Resources::E_TextureKey >::get_min() };
          textureKey < Enum< Resources::E_TextureKey >::get_max();
          ++textureKey )
    {
        sf::Texture texture {};
        std::string const textureLocalisation { get_texture_localisation(
            textureKey.get_value() ) };
        if ( ! texture.loadFromFile( textureLocalisation ) )
        {
            throw Exception::FileNotFound { textureLocalisation };
        }

        textures.insert( std::make_pair( textureKey.get_value(), texture ) );
    }
}

static void init_fonts(
    std::map< Resources::Resources::E_FontKey, sf::Font > & fonts )
{
    fonts.clear();

    for ( Enum< Resources::E_FontKey > fontKey {
              Enum< Resources::E_FontKey >::get_min() };
          fontKey < Enum< Resources::E_FontKey >::get_max();
          ++fontKey )
    {
        sf::Font font {};
        std::string const fontLocalisation { get_font_localisation(
            fontKey.get_value() ) };
        if ( ! font.loadFromFile( fontLocalisation ) )
        {
            throw Exception::FileNotFound { fontLocalisation };
        }

        fonts.insert( std::make_pair( fontKey.get_value(), font ) );
    }
}

static std::string get_texture_localisation(
    Resources::E_TextureKey const & textureKey )
{
    std::string imagePath { tools::get_path::resources() };

    switch ( textureKey )
    {
    case Resources::E_TextureKey::Tileset :
        imagePath += "/sprites/tileset/ground.png"s;
        break;
    case Resources::E_TextureKey::Player :
        imagePath += "/gold_sprite.png"s;
        break;
    case Resources::E_TextureKey::HomeWallpaper :
        imagePath += "/home_wallpaper.jpg"s;
        break;
    default :
        /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
        ASSERTION(
            false,
            "A texture of the Resources::E_TextureKey class hasn't been loaded"s
            "or the key doesn't exist in this enum"s );
        break;
    }

    return imagePath;
}

static std::string get_font_localisation( Resources::E_FontKey const & fontKey )
{
    std::string fontPath { tools::get_path::resources() };

    switch ( fontKey )
    {
    case Resources::E_FontKey::Arial :
        fontPath += "/arial.ttf"s;
        break;
    default :
        ASSERTION(
            false,
            "A texture of the Resources::E_TextureKey class hasn't been loaded"s
            "or the key doesn't exist in this enum"s );
        break;
    }

    return fontPath;
}