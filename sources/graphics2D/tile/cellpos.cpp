#include "cellpos.hpp"

#include <cstdlib>  // for div, (anonymous), div_t
#include <memory>   // for allocator
#include <sstream>

#include "maths/geometry/point.hpp"
#include "maths/vector2.tpp"    // for Vector2::operator=, floor, oper...
#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/traces.hpp"     // for Traces

namespace tile
{
    CellPos::CellPos( unsigned int index, unsigned int numberOfColumns )
      : Position {}, m_sizeX { numberOfColumns }
    {
        this->index( index );
    }

    CellPos::CellPos( unsigned int index, tile::Size tilemapSize )
      : CellPos { index, tilemapSize.tile().x }
    {}

    CellPos::CellPos( math::Vector2U position, unsigned int numberOfColumns,
                      Type type )
      : Position { position, type }, m_sizeX { numberOfColumns }
    {}

    CellPos::CellPos( math::Vector2U position, tile::Size tilemapSize,
                      Type type )
      : CellPos { position, tilemapSize.tile().x, type }
    {}

    CellPos::CellPos( tile::Position const & cell, unsigned int sizeX )
      : CellPos { cell.tile(), sizeX, Type::Tile }
    {}

    CellPos::CellPos( tile::Position const & cell, tile::Size tilemapSize )
      : CellPos { cell, tilemapSize.tile().x }
    {}

    unsigned int CellPos::index() const
    {
        math::Vector2U tilePos { this->tile() };
        return tilePos.y * this->sizeX() + tilePos.x;
    }

    void CellPos::index( unsigned int value )
    {
        this->set_tile_position(
            { value % this->sizeX(),
              static_cast< unsigned int >( math::whole_part(
                  static_cast< float >( value )
                  / static_cast< float >( this->sizeX() ) ) ) } );
    }

    unsigned int CellPos::sizeX() const
    {
        return m_sizeX;
    }

    Position CellPos::to_cell() const
    {
        return *this;
    }

    bool CellPos::is_inside( tile::Size const & size ) const
    {
        return this->tile().to_point().is_inside( { 0u, 0u }, size.tile() );
    }

    // ####################################################################
    // ########################## OPERATORS <=> ###########################
    // ####################################################################

    bool CellPos::operator<( tile::Size const & size ) const
    {
        return this->tile() < size.tile();
    }

    bool CellPos::operator>= ( tile::Size const & size ) const
    {
        return ! ( *this < size );
    }

    bool CellPos::operator<( tile::CellPos const & position ) const
    {
        return this->tile() < position.tile();
    }

    bool CellPos::operator>= ( tile::CellPos const & position ) const
    {
        return ! ( *this < position );
    }

    bool CellPos::operator== ( tile::CellPos const & position ) const
    {
        return this->tile() == position.tile()
               && this->sizeX() == position.sizeX();
    }

    bool CellPos::operator!= ( tile::CellPos const & position ) const
    {
        return ! ( *this == position );
    }

    // ####################################################################
    // ########################## OPERATORS +/- ###########################
    // ####################################################################

    CellPos operator+ ( tile::CellPos const & pos, tile::Size const & size )
    {
        math::Vector2U position = pos.tile() + size.tile();
        if ( position.x >= pos.sizeX() )
        {
            Trace::Warning( "tile::Position overflow" );
            position.x = pos.sizeX() - 1;
        }
        return CellPos { position, pos.sizeX(), Type::Tile };
    }

    CellPos operator- ( tile::CellPos const & pos, tile::Size const & size )
    {
        int value = static_cast< int >( pos.index() )
                    - static_cast< int >( size.index() );
        if ( value < 0 )
        {
            Trace::Warning( "tile::Position underflow" );
            value = 0;
        }
        return CellPos { static_cast< unsigned int >( value ), pos.sizeX() };
    }

    CellPos operator+ ( tile::CellPos const & posL, tile::CellPos const & posR )
    {
        math::Vector2U position = posL.tile() + posR.tile();
        if ( position.x >= posL.sizeX() )
        {
            Trace::Warning( "tile::Position overflow" );
            position.x = posL.sizeX() - 1;
        }
        return CellPos { position, posL.sizeX(), Type::Tile };
    }

    CellPos operator- ( tile::CellPos const & posL, tile::CellPos const & posR )
    {
        int value = static_cast< int >( posL.index() )
                    - static_cast< int >( posR.index() );
        if ( value < 0 )
        {
            Trace::Warning( "tile::Position underflow" );
            value = 0;
        }
        return CellPos { static_cast< unsigned int >( value ), posL.sizeX() };
    }

    CellPos & operator+= ( tile::CellPos & pos, tile::Size const & size )
    {
        return pos = pos + size;
    }

    CellPos & operator-= ( tile::CellPos & pos, tile::Size const & size )
    {
        return pos = pos - size;
    }

    CellPos & operator+= ( tile::CellPos & posL, tile::CellPos const & posR )
    {
        return posL = posL + posR;
    }

    CellPos & operator-= ( tile::CellPos & posL, tile::CellPos const & posR )
    {
        return posL = posL - posR;
    }

    void CellPos::set_tile_position( math::Vector2U position )
    {
        Position::set_tile_position( position );

        if ( this->tile().x >= m_sizeX )
        {  // Position too big for the current grid
            Trace::Warning( "Position {} X axis too big for {}", this->tile(),
                            m_sizeX );
        }
    }

}  // namespace tile
