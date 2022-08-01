#include "tile_position.hpp"

#include <cstdlib> // for div, (anonymous), div_t
#include <memory>  // for allocator

#include "maths/vector2.tpp"         // for Vector2::operator=, floor, oper...
#include "tools/assertion.hpp"       // for ASSERTION
#include "tools/global_variable.hpp" // for TILE_PIXEL_SIZE_VECTOR

TilePosition::TilePosition( unsigned int value, unsigned int numberOfTileInX )
  : m_value(), m_numberOfTileInX( numberOfTileInX )
{
    this->set_value( value );
}
TilePosition::TilePosition( math::Vector2U position,
                            unsigned int numberOfTileInX, Type type )
  : TilePosition { 0u, numberOfTileInX }
{
    this->set_value( position, type );
}

unsigned int TilePosition::value() const
{
    return this->m_value;
}

/// @brief convert the value to a tile position
math::Vector2U TilePosition::tile() const
{
    std::div_t divisionValue { std::div(
        static_cast< int >( this->m_value ),
        static_cast< int >( this->m_numberOfTileInX ) ) };
    return math::Vector2I { divisionValue.rem, divisionValue.quot }.to_u_int();
}
/// @brief convert the value to a pixel position
math::Vector2U TilePosition::pixel() const
{
    return this->tile() * TILE_PIXEL_SIZE_VECTOR;
}

void TilePosition::set_value( unsigned int newValue )
{
    this->m_value = newValue;
}
void TilePosition::set_value( math::Vector2U position, Type type )
{
    ASSERTION( type == Tile || type == Pixel, "Enum not handled" );

    math::Vector2U tilePosition { position };

    if ( type == Pixel )
    {
        // Convert the pixel position to tile position
        tilePosition =
            math::floor( position.to_float() / TILE_PIXEL_SIZE_VECTOR )
                .to_u_int();
    }

    this->m_value = tilePosition.x + ( tilePosition.y * m_numberOfTileInX );
}