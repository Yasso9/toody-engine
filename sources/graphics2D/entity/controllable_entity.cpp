#include "controllable_entity.hpp"

#include <vector>  // for vector

#include "maths/vector2.tpp"  // for operator*, Vector2::operator=, operator/

ControllableEntity2D::ControllableEntity2D(
    math::PolygonF quadrangle, Control control )
  : StaticEntity2D { quadrangle }, m_control { control }, m_speed {}

{
    this->set_speed( 500.f );
}

math::Vector2F ControllableEntity2D::get_speed() const
{
    return m_speed;
}

void ControllableEntity2D::set_speed( float speed )
{
    this->set_speed( { speed, speed } );
}

void ControllableEntity2D::set_speed( math::Vector2F speed )
{
    m_speed = speed;
}

void ControllableEntity2D::update( float deltaTime )
{
    math::Vector2F moveSpeed {
        ( m_speed / m_control.get_view().get_zoom() ) * deltaTime };
    math::Vector2F moveDirection { m_control.get_input_movement() };

    // ImGui::P_Show( "Entity Update Extra",
    //                 [moveDirection]()
    //                 {
    //                     std::stringstream output {};
    //                     output << "Move Direction : " << moveDirection <<
    //                     "\n"; ImGui::Text( "%s", output.str().c_str() );
    //                 } );

    this->move( moveSpeed * moveDirection );

    /// @todo ne pas revenir complètement, mais revenir à l'endroit juste avant
    /// la collision
    if ( m_control.is_collision_detected( this->get_polygon() ) )
    {
        this->move( -( moveSpeed * moveDirection ) );
    }
}