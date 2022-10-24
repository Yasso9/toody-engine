#include "static_entity.hpp"

#include <algorithm>  // for max, min
#include <sstream>    // for operator<<, basic_ostream:...
#include <string>     // for allocator, string, basic_s...
#include <vector>     // for vector

#include <IMGUI/imgui.h>                   // for InputFloat, Begin, Checkbox
#include <SFML/Graphics/Color.hpp>         // for Color, Color::Red
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget

#include "libraries/imgui.hpp"         // for P_ColorEditor, P_Begin
#include "main/render.hpp"             // for Render
#include "maths/geometry/point.hpp"    // for PointF
#include "maths/geometry/point.tpp"    // for Point::is_inside
#include "maths/geometry/polygon.tpp"  // for is_intersection, Polygon::...
#include "maths/geometry/segment.tpp"  // for Segment::is_intersected_by

namespace customisation
{
    namespace
    {
        static void circle_shape (
            std::string windowName, int shapeID, sf::CircleShape & circleShape )
        {
            if ( ImGui::Begin( windowName.c_str() ) )
            {
                ImGui::PushID( shapeID );

                sf::Color background { circleShape.getFillColor() };
                sf::Color outline { circleShape.getOutlineColor() };
                float outlineThickness { circleShape.getOutlineThickness() };
                float radius { circleShape.getRadius() };
                unsigned int numberOfPoint { static_cast< unsigned int >(
                    circleShape.getPointCount() ) };

                std::stringstream output {};
                output << "Shape n" << shapeID << "\n";
                ImGui::Text( "%s", output.str().c_str() );
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

                ImGui::PopID();
            }
            ImGui::End();
        }
    }  // namespace
}  // namespace customisation

StaticEntity2D::StaticEntity2D( math::PolygonF polygon )
  : Shape2D { polygon },
    m_ID {},
    m_isCustomisable { false },
    m_editPoint { false },
    m_pointShape { 5.f, 40 }
{
    static int instanceCount { 0 };
    m_ID = instanceCount;
    ++instanceCount;

    this->setFillColor( sf::Color::Red );
}

void StaticEntity2D::update( float /* deltaTime */ )
{
    if ( ! m_isCustomisable )
    {
        return;
    }

    ImGui::P_Show( "Customise Entity", &m_isCustomisable, [&] () {
        customisation::circle_shape( "Customise Entity", m_ID, m_pointShape );
        ImGui::Checkbox( "Edit Points ?", &m_editPoint );
    } );
}

void StaticEntity2D::render( Render & render ) const
{
    this->custom_draw( render );

    if ( ! m_isCustomisable )
    {
        return;
    }

    sf::CircleShape pointShape { m_pointShape };
    for ( math::PointF const & point : this->get_polygon().get_points() )
    {
        pointShape.setPosition( point.x, point.y );
        render.get_target().draw( pointShape, render.get_state() );
    }
}

void StaticEntity2D::set_customisation( bool isCustomisable )
{
    m_isCustomisable = isCustomisable;
}

bool StaticEntity2D::is_intersected_by(
    StaticEntity2D const & otherEntity ) const
{
    return math::is_intersection(
        this->get_polygon(), otherEntity.get_polygon() );
}
