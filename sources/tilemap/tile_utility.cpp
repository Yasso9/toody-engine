#include "tile_utility.hpp"

#include <cmath>

#include "tools/global_variable.hpp"

sf::Vector2u to_tile_position( unsigned int const tileNumber,
                               unsigned int const numberOfTileInX )
{
    return sf::Vector2u( ( tileNumber % numberOfTileInX ),
                         static_cast< unsigned int >(
                             std::floor( tileNumber / numberOfTileInX ) ) );
}

sf::Vector2u to_tile_position( sf::Vector2f const position )
{
    return sf::Vector2u( static_cast< unsigned int >(
                             std::floor( position.x / ::g_tileSize_f ) ),
                         static_cast< unsigned int >(
                             std::floor( position.y / ::g_tileSize_f ) ) );
}

namespace quad
{
    void set_position( sf::VertexArray & quad,
                       sf::Vector2f const & basePosition,
                       sf::Vector2u const & tilePosition )
    {
        // TYPO avoir directement un sf::Vector2f
        sf::Vector2f const tilePositionFloat { static_cast< sf::Vector2f >(
            tilePosition ) };

        quad[0].position = sf::Vector2f(
            basePosition.x + tilePositionFloat.x * ::g_tileSize_f,
            basePosition.y + tilePositionFloat.y * ::g_tileSize_f );
        quad[1].position = sf::Vector2f(
            basePosition.x + ( tilePositionFloat.x + 1.f ) * ::g_tileSize_f,
            basePosition.y + tilePositionFloat.y * ::g_tileSize_f );
        quad[2].position = sf::Vector2f(
            basePosition.x + ( tilePositionFloat.x + 1.f ) * ::g_tileSize_f,
            basePosition.y + ( tilePositionFloat.y + 1.f ) * ::g_tileSize_f );
        quad[3].position = sf::Vector2f(
            basePosition.x + tilePositionFloat.x * ::g_tileSize_f,
            basePosition.y + ( tilePositionFloat.y + 1.f ) * ::g_tileSize_f );
    }

    void set_texture_coordinate( sf::VertexArray & quad,
                                 sf::Vector2u const & tilePosition )
    {
        // TYPO avoir directement un sf::Vector2f
        sf::Vector2f const tilePositionFloat { static_cast< sf::Vector2f >(
            tilePosition ) };

        quad[0].texCoords =
            sf::Vector2f( tilePositionFloat.x * ::g_tileSize_f,
                          tilePositionFloat.y * ::g_tileSize_f );
        quad[1].texCoords =
            sf::Vector2f( ( tilePositionFloat.x + 1.f ) * ::g_tileSize_f,
                          tilePositionFloat.y * ::g_tileSize_f );
        quad[2].texCoords =
            sf::Vector2f( ( tilePositionFloat.x + 1.f ) * ::g_tileSize_f,
                          ( tilePositionFloat.y + 1.f ) * ::g_tileSize_f );
        quad[3].texCoords =
            sf::Vector2f( tilePositionFloat.x * ::g_tileSize_f,
                          ( tilePositionFloat.y + 1.f ) * ::g_tileSize_f );
    }

    void set_transparency( sf::VertexArray & quad )
    {
        for ( unsigned int i { 0u }; i < 4; ++i )
        {
            quad[i].color = sf::Color::Transparent;
        }
    }

    /* Rend visible le carré du quad (couleur noire) */
    void set_visible( sf::VertexArray & quad )
    {
        for ( unsigned int i { 0u }; i < 4; ++i )
        {
            quad[i].color = sf::Color( 255, 255, 255, 255 );
        }
    }
} // namespace quad
