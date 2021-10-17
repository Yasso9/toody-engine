#include "ressources_init.hpp"

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
                // Credit GragarLink10
                return "./ressources/sprites/tileset/ground.png";
                break;
            case TextureKey::Player :
                return "./ressources/sprites/players/gold.png";
                break;
            case TextureKey::HomeWallpaper :
                return "./ressources/home_wallpaper.jpg";
                break;
            default :
                /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
                throw std::runtime_error(
                    "The texture n"s
                    + std::to_string( static_cast<int>( textureKey ) )
                    + "of the TextureKey class hasn't been loaded"s
                      "or the key doesn't exist in this enum"s );
                break;
            }
        }

        std::string get_font_localisation( FontKey const & fontKey )
        {
            switch ( fontKey )
            {
            case FontKey::Arial :
                return "./ressources/arial.ttf";
                break;
            default :
                /* TYPO the best thing here is the possibility
                to use the c++20 std::format, but as of now
                g++ 11.2.0 doesn't support it */
                throw std::runtime_error(
                    "The font n" + std::to_string( static_cast<int>( fontKey ) )
                    + "of the FontKey class hasn't been loaded"
                      "or the key doesn't exist in this enum" );
                break;
            }
        }
    } // namespace

    std::map<TextureKey, sf::Texture> init_textures()
    {
        std::map<TextureKey, sf::Texture> textures {};

        for ( Enum<TextureKey> textureKey { Enum<TextureKey>::get_min() };
              textureKey < Enum<TextureKey>::get_max();
              ++textureKey )
        {
            sf::Texture texture {};
            std::string const textureLocalisation { get_texture_localisation(
                textureKey.get_value() ) };
            if ( ! texture.loadFromFile( textureLocalisation ) )
            {
                throw std::runtime_error( "There isn't a file named \""
                                          + textureLocalisation + "\"" );
            }

            textures.insert(
                std::make_pair( textureKey.get_value(), texture ) );
        }

        return textures;
    }

    // TYPO change this function to make it look like init_textures
    std::map<FontKey, sf::Font> init_fonts()
    {
        std::map<FontKey, sf::Font> fonts {};

        for ( int fontKey { 0 };
              fontKey < static_cast<int>( FontKey::EnumLast );
              ++fontKey )
        {
            sf::Font font {};
            std::string const fontLocalisation { get_font_localisation(
                static_cast<FontKey>( fontKey ) ) };
            if ( ! font.loadFromFile( fontLocalisation ) )
            {
                throw std::runtime_error(
                    "The file \"" + fontLocalisation
                    + "\" to load the font does not exist anymore" );
            }

            fonts.insert(
                std::make_pair( static_cast<FontKey>( fontKey ), font ) );
        }

        return fonts;
    }
} // namespace RessourcesInit