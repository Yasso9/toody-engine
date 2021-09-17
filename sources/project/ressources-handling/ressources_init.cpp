#include "ressources_init.hpp"

namespace RessourcesInit
{
    namespace
    {
        template <typename T>
        // TYPO pas trop compris ce que ça fait
        T & increment( T & value )
        {
            assert( std::is_integral< std::underlying_type_t<T> >::value && "Can't increment value" );

            ++((std::underlying_type_t<T> &) value);
            return value;
        }
    }

    std::map< TextureKey, sf::Texture > init_textures()
    {
        std::map< TextureKey, sf::Texture > textures {};

        for (
            TextureKey textureKey { static_cast<TextureKey>(0) };
            textureKey < TextureKey::Count;
            increment( textureKey )
        )
        {
            std::cout << "TextureKey" << std::endl;
            std::cout << static_cast<int>(textureKey) << std::endl;

            textures.insert( std::make_pair( textureKey, sf::Texture() ) );
        }

        if ( !textures.at( TextureKey::Player ).loadFromFile( "./ressources/sprites/players/gold.png" ) ) // Credit GragarLink10
        {
            throw std::runtime_error( "There isn't a file named \"Sprites/Players/Gold.png\"" );
        }
        if ( !textures.at( TextureKey::Player ).loadFromFile( "./ressources/sprites/tileset/tileset.png" ) )
        {
            throw std::runtime_error( "There isn't a file named \"Sprites/Tileset/tileset.png\"" );
        }

        return textures;
    }

    std::map< FontKey, sf::Font > init_fonts()
    {
        sf::Font fontArial {};

        if ( !fontArial.loadFromFile( "./ressources/arial.ttf" ) )
        {
            throw std::runtime_error( "There isn't a file named \"Police/arial.ttf\"" );
        }

        return {
            { FontKey::Arial, fontArial },
        };
    }
}