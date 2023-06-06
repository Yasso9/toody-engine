#include "position.hpp"

#include <cstdlib>  // for div, (anonymous), div_t
#include <memory>   // for allocator
#include <sstream>

#include "maths/geometry/point.hpp"
#include "maths/vector2.tpp"    // for Vector2::operator=, floor, oper...
#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/traces.hpp"     // for Traces

namespace tile
{
    Position::Position( unsigned int index, unsigned int numberOfColumns )
      : Cell {}, m_sizeX { numberOfColumns }
    {
        this->index( index );
    }

    Position::Position( unsigned int index, tile::Size mapSize )
      : Position { index, mapSize.tile().x }
    {}

    Position::Position( math::Vector2U position, unsigned int numberOfColumns,
                        Type type )
      : Cell { position, type }, m_sizeX { numberOfColumns }
    {}

    Position::Position( math::Vector2U position, tile::Size mapSize, Type type )
      : Position { position, mapSize.tile().x, type }
    {}

    unsigned int Position::index() const
    {
        math::Vector2U tilePos { this->tile() };
        return tilePos.y * this->sizeX() + tilePos.x;
    }

    void Position::index( unsigned int value )
    {
        this->set_tile_position(
            { value % this->sizeX(),
              static_cast< unsigned int >( math::whole_part(
                  static_cast< float >( value )
                  / static_cast< float >( this->sizeX() ) ) ) } );
    }

    unsigned int Position::sizeX() const
    {
        return m_sizeX;
    }

    Cell Position::to_cell() const
    {
        return *this;
    }

    bool Position::is_inside( tile::Size const & size ) const
    {
        return this->tile().to_point().is_inside( { 0u, 0u }, size.tile() );
    }

    // ####################################################################
    // ########################## OPERATORS <=> ###########################
    // ####################################################################

    bool Position::operator<( tile::Size const & size ) const
    {
        return this->tile() < size.tile();
    }

    bool Position::operator>= ( tile::Size const & size ) const
    {
        return ! ( *this < size );
    }

    bool Position::operator<( tile::Position const & position ) const
    {
        return this->tile() < position.tile();
    }

    bool Position::operator>= ( tile::Position const & position ) const
    {
        return ! ( *this < position );
    }

    bool Position::operator== ( tile::Position const & position ) const
    {
        return this->tile() == position.tile()
               && this->sizeX() == position.sizeX();
    }

    bool Position::operator!= ( tile::Position const & position ) const
    {
        return ! ( *this == position );
    }

    // ####################################################################
    // ########################## OPERATORS +/- ###########################
    // ####################################################################

    Position operator+ ( tile::Position const & pos, tile::Size const & size )
    {
        math::Vector2U position = pos.tile() + size.tile();
        if ( position.x >= pos.sizeX() )
        {
            Trace::Warning( "tile::Position overflow" );
            position.x = pos.sizeX() - 1;
        }
        return Position { position, pos.sizeX(), Type::Tile };
    }

    Position operator- ( tile::Position const & pos, tile::Size const & size )
    {
        int value = static_cast< int >( pos.index() )
                    - static_cast< int >( size.index() );
        if ( value < 0 )
        {
            Trace::Warning( "tile::Position underflow" );
            value = 0;
        }
        return Position { static_cast< unsigned int >( value ), pos.sizeX() };
    }

    Position operator+ ( tile::Position const & posL,
                         tile::Position const & posR )
    {
        math::Vector2U position = posL.tile() + posR.tile();
        if ( position.x >= posL.sizeX() )
        {
            Trace::Warning( "tile::Position overflow" );
            position.x = posL.sizeX() - 1;
        }
        return Position { position, posL.sizeX(), Type::Tile };
    }

    Position operator- ( tile::Position const & posL,
                         tile::Position const & posR )
    {
        int value = static_cast< int >( posL.index() )
                    - static_cast< int >( posR.index() );
        if ( value < 0 )
        {
            Trace::Warning( "tile::Position underflow" );
            value = 0;
        }
        return Position { static_cast< unsigned int >( value ), posL.sizeX() };
    }

    Position & operator+= ( tile::Position & pos, tile::Size const & size )
    {
        return pos = pos + size;
    }

    Position & operator-= ( tile::Position & pos, tile::Size const & size )
    {
        return pos = pos - size;
    }

    Position & operator+= ( tile::Position & posL, tile::Position const & posR )
    {
        return posL = posL + posR;
    }

    Position & operator-= ( tile::Position & posL, tile::Position const & posR )
    {
        return posL = posL - posR;
    }

    void Position::set_tile_position( math::Vector2U position )
    {
        Cell::set_tile_position( position );

        if ( this->tile().x >= m_sizeX )
        {  // Position too big for the current grid
            Trace::Warning( "Position {} X axis too big for {}", this->tile(),
                            m_sizeX );
        }
    }

}  // namespace tile
