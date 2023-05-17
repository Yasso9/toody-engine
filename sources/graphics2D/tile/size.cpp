#include "size.hpp"

#include "graphics2D/tile/tools.hpp"

namespace tile
{
    Size::Size( math::Vector2U size, Type type ) : m_size {}
    {
        this->set_value( size, type );
    }

    unsigned int Size::value() const
    {
        return m_size.x * m_size.x + m_size.y;
    }

    math::Vector2U Size::tile() const
    {
        return m_size;
    }

    math::Vector2U Size::pixel() const
    {
        return tile::tile_to_pixel_position( this->tile() );
    }

    void Size::set_value( math::Vector2U position, Type type )
    {
        m_size = position;

        if ( type == Type::Pixel )
        {
            m_size = tile::pixel_to_tile_position( position );
        }
    }
}  // namespace tile