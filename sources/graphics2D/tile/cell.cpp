#include "cell.hpp"

#include "tools/traces.hpp"  // for Trace::Warning

namespace tile
{
    Cell::Cell() : Cell { { 0, 0 }, Type::Tile } {}

    Cell::Cell( math::Vector2U position, Type type )
      : m_tilePosition {}, m_tileSizePixel { 32, 32 }
    {
        this->position( position, type );
    }

    Cell::Cell( unsigned int x, unsigned int y, Type type )
      : Cell { { x, y }, type }
    {}

    math::Vector2U Cell::tile() const
    {
        return m_tilePosition;
    }

    math::Vector2U Cell::pixel() const
    {
        return m_tilePosition * m_tileSizePixel;
    }

    void Cell::tile( math::Vector2U position )
    {
        this->set_tile_position( position );
    }

    void Cell::pixel( math::Vector2U position )
    {
        this->set_tile_position(
            math::whole_part( position.to_float() / m_tileSizePixel.to_float() )
                .to_uint() );
    }

    void Cell::position( math::Vector2U position, Type type )
    {
        if ( type == Type::Tile )
        {
            this->tile( position );
        }
        else if ( type == Type::Pixel )
        {
            this->pixel( position );
        }
        else
        {
            Trace::Warning( "Cell::set_value: type not handled" );
        }
    }

    void Cell::position( unsigned int x, unsigned int y, Type type )
    {
        this->position( { x, y }, type );
    }

    void Cell::set_tile_position( math::Vector2U position )
    {
        m_tilePosition = position;
    }
}  // namespace tile
