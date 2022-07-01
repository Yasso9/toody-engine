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
     * | 14 | 15 | 16 | 17 | 18 | 19 | 20 | */
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

    TilePosition( unsigned int value, unsigned int numberOfTileInX );
    TilePosition( math::Vector2U position, unsigned int numberOfTileInX,
                  Type type );

    unsigned int value() const;

    /// @brief convert the value to a tile position
    math::Vector2U tile() const;
    /// @brief convert the value to a pixel position
    math::Vector2U pixel() const;

    void set_value( unsigned int newValue );
    void set_value( math::Vector2U position, Type type );
};