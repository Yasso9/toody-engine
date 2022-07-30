#include "moveable_entity.hpp"

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

MoveableEntity2D::MoveableEntity2D( math::PolygonF quadrangle,
                                    CollisionList const & collisionList,
                                    View const & view,
                                    input::S_KeyboardMove movementKey )
  : StaticEntity2D( quadrangle ),
    m_collisionList( collisionList ),
    m_view( view ),
    m_movementKey( movementKey ),
    m_speed()
{
    this->set_speed( 500.f );
}

math::Vector2F MoveableEntity2D::get_speed() const
{
    return m_speed;
}

void MoveableEntity2D::set_speed( float speed )
{
    this->m_speed = { speed, speed };
}
void MoveableEntity2D::set_speed( math::Vector2F speed )
{
    this->m_speed = speed;
}

bool MoveableEntity2D::is_collision_detected() const
{
    for ( StaticEntity2D const & entity : m_collisionList.get_array() )
    {
        if ( this->is_intersected_by( entity ) )
        {
            return true;
        }
    }
    return false;
}

void MoveableEntity2D::update_before( float deltaTime )
{
    math::Vector2F moveSpeed { ( m_speed / m_view.get_zoom() ) * deltaTime };
    math::Vector2F moveDirection { input::get_movement_vector(
        m_movementKey ) };

    // ImGui::P_Begin( "Entity Update Extra",
    //                 [moveDirection]()
    //                 {
    //                     std::stringstream output {};
    //                     output << "Move Direction : " << moveDirection << "\n";
    //                     ImGui::Text( "%s", output.str().c_str() );
    //                 } );

    this->move( moveSpeed * moveDirection );

    if ( this->is_collision_detected() )
    {
        this->move( -( moveSpeed * moveDirection ) );
    }
}