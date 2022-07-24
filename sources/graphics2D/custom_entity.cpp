#include "custom_entity.hpp"

namespace customisation
{
    namespace
    {
        static void circle_shape( std::string windowName,
                                  sf::CircleShape & circleShape )
        {
            if ( ImGui::Begin( windowName.c_str() ) )
            {
                sf::Color background { circleShape.getFillColor() };
                sf::Color outline { circleShape.getOutlineColor() };
                float outlineThickness { circleShape.getOutlineThickness() };
                float radius { circleShape.getRadius() };
                unsigned int numberOfPoint { static_cast< unsigned int >(
                    circleShape.getPointCount() ) };

                ImGui::P_ColorEditor( "Background Color", background );
                ImGui::P_ColorEditor( "Outline Color", outline );
                ImGui::InputFloat( "Outline Thickness", &outlineThickness );
                ImGui::InputFloat( "Radius", &radius );
                ImGui::P_InputNumber( "Number of Point", numberOfPoint );

                circleShape.setFillColor( background );
                circleShape.setOutlineColor( outline );
                circleShape.setOutlineThickness( outlineThickness );
                circleShape.setRadius( radius );
                circleShape.setPointCount( numberOfPoint );
            }
            ImGui::End();
        }
    } // namespace
} // namespace customisation

CustomEntity2D::CustomEntity2D( math::PolygonF quadrangle )
  : StaticEntity2D( quadrangle ),
    m_showCustomisation( true ),
    m_editPoint( false ),
    m_pointShape { 5.f, 40 }
{}

void CustomEntity2D::update_before( float /* deltaTime */ )
{
    if ( ! m_showCustomisation )
    {
        return;
    }

    if ( ImGui::Begin( "Customise Entity", &m_showCustomisation ) )
    {
        customisation::circle_shape( "Customise Entity", m_pointShape );

        ImGui::Checkbox( "Edit Points ?", &m_editPoint );
    }
    ImGui::End();
}

void CustomEntity2D::render_before( Render & render ) const
{
    StaticEntity2D::render_before( render );

    sf::CircleShape pointShape { m_pointShape };
    for ( math::PointF const & point : this->get_polygon().get_points() )
    {
        pointShape.setPosition( point.x, point.y );
        render.get_target().draw( pointShape, render.get_state() );
    }
}