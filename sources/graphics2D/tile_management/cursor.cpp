#include "cursor.hpp"

#include "tools/global_variable.hpp"

namespace tile
{
    Cursor::Cursor() : shape {}
    {
        // Tilemap Cursor Initialisation
        shape.setSize( TILE_PIXEL_SIZE_VECTOR );
        shape.setOutlineThickness( -3.f );
        shape.setOutlineColor( sf::Color::Transparent );
        shape.setFillColor( sf::Color::Transparent );
        shape.setPosition( 0.f, 0.f );
    }
}  // namespace tile