#pragma once

#include "graphics2D/tile/position.hpp"  // for Cell
#include "graphics2D/tile/size.hpp"      // for Size
#include "maths/vector2.hpp"             // for Vector2U

namespace tile
{
    class CellPos : public Position
    {
        // number of tile in the X axis of the map/grid where the tile is
        // positionned
        unsigned int m_sizeX;

      public:
        CellPos( unsigned int index, unsigned int numberOfColumns );
        CellPos( unsigned int index, tile::Size mapSize );
        CellPos( math::Vector2U position, unsigned int numberOfColumns,
                 Type type );
        CellPos( math::Vector2U position, tile::Size mapSize, Type type );
        CellPos( tile::Position const &, unsigned int sizeX );
        CellPos( tile::Position const &, tile::Size tilemapSize );

        // get index of the tile
        unsigned int index () const;
        void         index ( unsigned int newValue );

        unsigned int sizeX () const;

        Position to_cell () const;

        bool is_inside ( tile::Size const & size ) const;

        // ####################################################################
        // ########################## OPERATORS <=> ###########################
        // ####################################################################

        bool operator<( tile::Size const & size ) const;
        bool operator>= ( tile::Size const & size ) const;
        bool operator<( tile::CellPos const & position ) const;
        bool operator>= ( tile::CellPos const & position ) const;
        // Compare position AND numberOfColumns
        bool operator== ( tile::CellPos const & position ) const;
        bool operator!= ( tile::CellPos const & position ) const;

        // ####################################################################
        // ########################## OPERATORS +/- ###########################
        // ####################################################################

        friend CellPos   operator+ ( tile::CellPos const & pos,
                                   tile::Size const &    size );
        friend CellPos   operator- ( tile::CellPos const & pos,
                                   tile::Size const &    size );
        friend CellPos   operator+ ( tile::CellPos const & posL,
                                   tile::CellPos const & posR );
        friend CellPos   operator- ( tile::CellPos const & posL,
                                   tile::CellPos const & posR );
        friend CellPos & operator+= ( tile::CellPos &    pos,
                                      tile::Size const & size );
        friend CellPos & operator-= ( tile::CellPos &    pos,
                                      tile::Size const & size );
        friend CellPos & operator+= ( tile::CellPos &       posL,
                                      tile::CellPos const & posR );
        friend CellPos & operator-= ( tile::CellPos &       posL,
                                      tile::CellPos const & posR );

      private:
        void set_tile_position ( math::Vector2U position ) override;
    };

}  // namespace tile

DEFINE_FORMATTER( tile::CellPos, "{}", value.to_cell() );
