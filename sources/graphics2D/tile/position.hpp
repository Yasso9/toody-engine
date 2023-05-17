#pragma once

#include "graphics2D/tile/size.hpp"
#include "maths/vector2.hpp"  // for Vector2U

namespace tile
{
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
        Position( math::Vector2U position, unsigned int numberOfColumns,
                  Type type );
        Position( math::Vector2U position, tile::Size mapSize, Type type );

        /// @brief get index of the tile
        unsigned int   value () const;
        /// @brief get position in tile value
        math::Vector2U tile () const;
        /// @brief get position in pixel value
        math::Vector2U pixel () const;

        unsigned int get_number_of_columns () const;

        void set_value ( unsigned int newValue );
        void set_value ( math::Vector2U position, Type type );

        bool is_inside ( tile::Size size ) const;

        bool operator<( tile::Size size ) const;
        bool operator>= ( tile::Size size ) const;
        bool operator<( tile::Position position ) const;
        bool operator>= ( tile::Position position ) const;
        /// @brief Compare position AND numberOfColumns
        bool operator== ( tile::Position position ) const;

        std::string debug_string ( std::string name = "" ) const;
    };
}  // namespace tile