#include "cursor.hpp"

namespace tile
{
    Cursor::Cursor() : m_shape {}
    {
        // Tilemap Cursor Initialisation
        m_shape.setSize( TILE_PIXEL_SIZE_VECTOR );
        m_shape.setOutlineThickness( -3.f );
        m_shape.setOutlineColor( sf::Color::Transparent );
        m_shape.setFillColor( sf::Color::Transparent );
        m_shape.setPosition( 0.f, 0.f );
    }
}  // namespace tile