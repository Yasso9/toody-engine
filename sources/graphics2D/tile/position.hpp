#pragma once

#include "graphics2D/tile/cell.hpp"
#include "graphics2D/tile/size.hpp"
#include "maths/vector2.hpp"  // for Vector2U

namespace tile
{
    class Position : public Cell
    {
        // number of tile in the X axis of the map/grid where the tile is
        // positionned
        unsigned int m_sizeX;

      public:
        Position( unsigned int index, unsigned int numberOfColumns );
        Position( unsigned int index, tile::Size mapSize );
        Position( math::Vector2U position, unsigned int numberOfColumns,
                  Type type );
        Position( math::Vector2U position, tile::Size mapSize, Type type );

        // get index of the tile
        unsigned int index () const;
        void         index ( unsigned int newValue );

        unsigned int sizeX () const;

        Cell to_cell () const;

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

      private:
        void set_tile_position ( math::Vector2U position ) override;
    };

}  // namespace tile

DEFINE_FORMATTER( tile::Position, "{}", value.to_cell() );
