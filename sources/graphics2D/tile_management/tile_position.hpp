#pragma once

#include "maths/vector2.hpp"  // for Vector2U

namespace tile
{
    /// @todo the Position and Size classes must be interdependent
    class Size;

    class Position
    {
        /// @brief Position in tile value
        math::Vector2U m_position;
        /// @brief number of tile in the X axis of the map/grid
        ///        where the tile is positionned
        unsigned int   m_numberOfColumns;

      public:
        enum Type
        {
            Tile = 0,
            Pixel
        };

        Position( unsigned int value, unsigned int numberOfColumns );
        Position( unsigned int value, tile::Size mapSize );
        Position(
            math::Vector2U position, unsigned int numberOfColumns, Type type );
        Position( math::Vector2U position, tile::Size mapSize, Type type );

        unsigned int   value () const;
        /// @brief get size in tile value
        math::Vector2U tile () const;
        /// @brief get size in pixel value
        math::Vector2U pixel () const;

        void set_value ( unsigned int newValue );
        void set_value ( math::Vector2U position, Type type );
    };

    class Size
    {
        /// @brief Size in tile value
        math::Vector2U m_size;

      public:
        enum Type
        {
            Tile = 0,
            Pixel
        };

        Size( math::Vector2U size, Type type );

        unsigned int   value () const;
        math::Vector2U tile () const;
        math::Vector2U pixel () const;

        void set_value ( math::Vector2U position, Type type );
    };
}  // namespace tile