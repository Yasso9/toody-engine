#pragma once

#include "graphics2D/tile/size.hpp"
#include "maths/vector2.hpp"  // for Vector2U

namespace tile
{
    class Position
    {
        // math::Vector2U const TILE_SIZE { 32.f, 32.f };

        // Position in tile value
        math::Vector2U m_position;
        // number of tile in the X axis of the map/grid where the tile is
        // positionned
        unsigned int m_numberOfColumns;

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

        // get index of the tile
        unsigned int value () const;
        // get position in tile value
        math::Vector2U tile () const;
        // position in pixel value
        math::Vector2U pixel () const;

        unsigned int get_number_of_columns () const;

        void set_value ( unsigned int newValue );
        void set_value ( math::Vector2U position, Type type );

        bool is_inside ( tile::Size const & size ) const;

        // ####################################################################
        // ########################## OPERATORS <=> ###########################
        // ####################################################################

        bool operator<( tile::Size const & size ) const;
        bool operator>= ( tile::Size const & size ) const;
        bool operator<( tile::Position const & position ) const;
        bool operator>= ( tile::Position const & position ) const;
        // Compare position AND numberOfColumns
        bool operator== ( tile::Position const & position ) const;
        bool operator!= ( tile::Position const & position ) const;

        // ####################################################################
        // ########################## OPERATORS +/- ###########################
        // ####################################################################

        friend Position   operator+ ( tile::Position const & pos,
                                    tile::Size const &     size );
        friend Position   operator- ( tile::Position const & pos,
                                    tile::Size const &     size );
        friend Position   operator+ ( tile::Position const & posL,
                                    tile::Position const & posR );
        friend Position   operator- ( tile::Position const & posL,
                                    tile::Position const & posR );
        friend Position & operator+= ( tile::Position &   pos,
                                       tile::Size const & size );
        friend Position & operator-= ( tile::Position &   pos,
                                       tile::Size const & size );
        friend Position & operator+= ( tile::Position &       posL,
                                       tile::Position const & posR );
        friend Position & operator-= ( tile::Position &       posL,
                                       tile::Position const & posR );

        std::string debug_string ( std::string name = "" ) const;
    };

}  // namespace tile

DEFINE_FORMATTER( tile::Position, "{{ tile={} }}", value.tile() );
