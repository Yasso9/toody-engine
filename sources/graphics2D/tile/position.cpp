#include "position.hpp"

#include "tools/traces.hpp"  // for Trace::Warning

namespace tile
{
    Position::Position() : Position { { 0, 0 }, Type::Tile } {}

    Position::Position( math::Vector2U position, Type type )
      : m_tilePosition {}, m_tileSizePixel { 32, 32 }
    {
        this->position( position, type );
    }

    Position::Position( unsigned int x, unsigned int y, Type type )
      : Position { { x, y }, type }
    {}

    math::Vector2U Position::tile() const
    {
        return m_tilePosition;
    }

    math::Vector2U Position::pixel() const
    {
        return m_tilePosition * m_tileSizePixel;
    }

    math::Vector2U Position::pixel_abs( math::Vector2F const & gridPos ) const
    {
        return this->pixel() + gridPos;
    }

    void Position::tile( math::Vector2U position )
    {
        this->set_tile_position( position );
    }

    void Position::pixel( math::Vector2U position )
    {
        this->set_tile_position(
            math::whole_part( position.to_float() / m_tileSizePixel.to_float() )
                .to_uint() );
    }

    void Position::position( math::Vector2U position, Type type )
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

    void Position::position( unsigned int x, unsigned int y, Type type )
    {
        this->position( { x, y }, type );
    }

    void Position::set_tile_position( math::Vector2U position )
    {
        m_tilePosition = position;
    }
}  // namespace tile
