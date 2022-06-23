#pragma once

#include "maths/maths.hpp"
#include "tools/assertion.hpp"
#include "tools/global_variable.hpp"

class TilePosition
{
    /** @brief value of the tilemap
     * example of possible values in a 7<3 tilemap
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
     * | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
     * | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
    */
    unsigned int m_value;
    /// @brief number of tile in the X axis of the map/grid
    ///        where the tile is positionned
    unsigned int m_numberOfTileInX;

  public:
    enum Type
    {
        Tile = 0,
        Pixel
    };

    TilePosition( unsigned int aValue, unsigned int aNumberOfTileInX )
      : m_value( aValue ), m_numberOfTileInX( aNumberOfTileInX )
    {
        this->set_value( aValue );
    }
    TilePosition( math::Vector2U position, unsigned int aNumberOfTileInX,
                  Type type )
      : TilePosition { 0u, aNumberOfTileInX }
    {
        this->set_value( position, type );
    }

    unsigned int value() const { return this->m_value; }

    /// @brief convert the value to a tile position
    math::Vector2U tile() const
    {
        std::div_t divisionValue { std::div(
            static_cast< int >( this->m_value ),
            static_cast< int >( this->m_numberOfTileInX ) ) };
        return math::Vector2I { divisionValue.rem, divisionValue.quot }
            .to_u_int();
    }
    /// @brief convert the value to a pixel position
    math::Vector2U pixel() const
    {
        return this->tile() * TILE_PIXEL_SIZE_VECTOR;
    }

    void set_value( unsigned int newValue ) { this->m_value = newValue; }
    void set_value( math::Vector2U position, Type type )
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
};