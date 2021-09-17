#include "tile_utility.hpp"

sf::Vector2u to_tile_position( unsigned int const tileNumber, unsigned int const numberOfTileInX )
{
    return sf::Vector2u(
        (tileNumber % numberOfTileInX),
        static_cast<unsigned int>(std::floor( tileNumber / numberOfTileInX ))
    );
}

sf::Vector2u to_tile_position( sf::Vector2f const position )
{
    return sf::Vector2u(
        static_cast<unsigned int>(std::floor( position.x / g_squareSize )),
        static_cast<unsigned int>(std::floor( position.y / g_squareSize ))
    );
}

bool is_in_part( sf::Vector2f const value, sf::Vector2f const position, sf::Vector2f const size )
{
    return (
        value.x >= position.x &&
        value.x < position.x + size.x &&
        value.y >= position.y &&
        value.y < position.y + size.y
    );
}


void set_quad_position( sf::Vertex *& quad, sf::Vector2f const & basePosition, sf::Vector2u const & tilePosition )
{
    // TYPO avoir directement un sf::Vector2f
    sf::Vector2f const tilePositionFloat { static_cast<sf::Vector2f>(tilePosition) };

    quad[0].position = sf::Vector2f(
        basePosition.x + tilePositionFloat.x * g_squareSize,
        basePosition.y + tilePositionFloat.y * g_squareSize
    );
    quad[1].position = sf::Vector2f(
        basePosition.x + (tilePositionFloat.x + 1.f) * g_squareSize,
        basePosition.y + tilePositionFloat.y * g_squareSize
    );
    quad[2].position = sf::Vector2f(
        basePosition.x + (tilePositionFloat.x + 1.f) * g_squareSize,
        basePosition.y + (tilePositionFloat.y + 1.f) * g_squareSize
    );
    quad[3].position = sf::Vector2f(
        basePosition.x + tilePositionFloat.x * g_squareSize,
        basePosition.y + (tilePositionFloat.y + 1.f) * g_squareSize
    );
}

void set_quad_texture_coordinate( sf::Vertex *& quad, sf::Vector2u const & tilePosition )
{
    // TYPO avoir directement un sf::Vector2f
    sf::Vector2f const tilePositionFloat { static_cast<sf::Vector2f>(tilePosition) };

    quad[0].texCoords = sf::Vector2f(
        tilePositionFloat.x * g_squareSize,
        tilePositionFloat.y * g_squareSize
    );
    quad[1].texCoords = sf::Vector2f(
        (tilePositionFloat.x + 1.f) * g_squareSize,
        tilePositionFloat.y * g_squareSize
    );
    quad[2].texCoords = sf::Vector2f(
        (tilePositionFloat.x + 1.f) * g_squareSize,
        (tilePositionFloat.y + 1.f) * g_squareSize
    );
    quad[3].texCoords = sf::Vector2f(
        tilePositionFloat.x * g_squareSize,
        (tilePositionFloat.y + 1.f) * g_squareSize
    );
}

void set_quad_transparency( sf::Vertex *& quad )
{
    for ( unsigned int i { 0u }; i < 4; ++i )
    {
        quad[i].color = sf::Color::Transparent;
    }
}


/* Rend visible le carré du quad (couleur noire) */
void set_quad_visible( sf::Vertex *& quad )
{
    for ( unsigned int i { 0u }; i < 4; ++i )
    {
        quad[i].color = sf::Color( 255, 255, 255, 255 );
    }
}
