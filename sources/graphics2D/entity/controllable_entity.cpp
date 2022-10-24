#include "controllable_entity.hpp"

#include <vector>  // for vector

#include "maths/vector2.tpp"  // for operator*, Vector2::operator=, operator/

ControllableEntity2D::ControllableEntity2D(
    math::PolygonF quadrangle, CollisionList const & collisionList,
    View const & view, input::S_KeyboardMove movementKey )
  : StaticEntity2D( quadrangle ),
    m_collisionList( collisionList ),
    m_view( view ),
    m_movementKey( movementKey ),
    m_speed()
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

bool ControllableEntity2D::is_collision_detected() const
{
    for ( StaticEntity2D const & entity : m_collisionList.get_entities() )
    {
        if ( this->is_intersected_by( entity ) )
        {
            return true;
        }
    }
    return false;
}

void ControllableEntity2D::update( float deltaTime )
{
    math::Vector2F moveSpeed { ( m_speed / m_view.get_zoom() ) * deltaTime };
    math::Vector2F moveDirection {
        input::get_movement_vector( m_movementKey ) };

    // ImGui::P_Show( "Entity Update Extra",
    //                 [moveDirection]()
    //                 {
    //                     std::stringstream output {};
    //                     output << "Move Direction : " << moveDirection <<
    //                     "\n"; ImGui::Text( "%s", output.str().c_str() );
    //                 } );

    this->move( moveSpeed * moveDirection );

    if ( this->is_collision_detected() )
    {
        this->move( -( moveSpeed * moveDirection ) );
    }
}