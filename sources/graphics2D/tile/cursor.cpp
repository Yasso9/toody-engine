#include "cursor.hpp"

#include "graphics2D/constants.hpp"

namespace tile
{
    Cursor::Cursor( Type type ) : m_shape {}, m_type { type }
    {
        sf::Color baseColor { 48, 48, 48 };

        m_shape.setSize( TILE_PIXEL_SIZE_VECTOR );
        m_shape.setOutlineThickness( -3.f );
        m_shape.setPosition( 0.f, 0.f );
        m_shape.setFillColor( sf::Color::Transparent );
        m_shape.setOutlineColor( sf::Color::Transparent );

        this->set_color( baseColor );

        this->hide();
    }

    void Cursor::render( Render & render ) const
    {
        render.draw( m_shape );
    }

    void Cursor::hide()
    {
        sf::Color color { m_shape.getOutlineColor() };
        color.a = 0;
        this->set_color( color );
    }

    void Cursor::show_at_position( math::Vector2F position )
    {
        sf::Color color { m_shape.getOutlineColor() };
        color.a = 128;
        this->set_color( color );
        m_shape.setPosition( position );
    }

    void Cursor::set_color( sf::Color color )
    {
        if ( m_type == Full )
        {
            m_shape.setFillColor( color );
        }
        else if ( m_type == Outline )
        {
            m_shape.setOutlineColor( color );
        }
    }
}  // namespace tile