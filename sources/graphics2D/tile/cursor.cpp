#include "cursor.hpp"

#include "graphics2D/constants.hpp"
#include "graphics2D/tile/map.hpp"

namespace tile
{
    MouseCursor::MouseCursor( Type type )
      : m_shape {}, m_type { type }, m_on_click {}
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

    void MouseCursor::manual_update( UpdateContext &   context,
                                     tile::Map const & tilemap )
    {
        math::PointF mousePosition {
            context.inputs.get_mouse_position( tilemap.get_view() )
                .to_point() };

        std::optional< tile::Position > tilePosition {
            tilemap.get_position( mousePosition ) };

        if ( ! tilePosition.has_value()
             || ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) )
        {  // The mouse is outside the tilemap
            this->hide();
            return;
        }

        this->show();
        this->set_position( tilePosition.value().pixel().to_float() );

        if ( context.inputs.is_pressed( sf::Mouse::Button::Left ) )
        {
            m_on_click( tilePosition.value() );
        }
    }

    void MouseCursor::render( RenderContext & context ) const
    {
        context.draw( m_shape );
    }

    void MouseCursor::hide()
    {
        sf::Color color { m_shape.getOutlineColor() };
        color.a = 0;
        this->set_color( color );
    }

    void MouseCursor::show()
    {
        sf::Color color { m_shape.getOutlineColor() };
        color.a = 128;
        this->set_color( color );
    }

    void MouseCursor::set_position( math::Vector2F position )
    {
        m_shape.setPosition( position );
    }

    void MouseCursor::on_click(
        std::function< void( tile::Position ) > callback )
    {
        m_on_click = callback;
    }

    void MouseCursor::set_color( sf::Color color )
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
