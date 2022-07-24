#include "object.hpp"

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

StaticEntity2D::StaticEntity2D( math::PolygonF polygon ) : Shape2D { polygon }
{
    this->setFillColor( sf::Color::Red );
}

bool StaticEntity2D::is_intersected_by(
    StaticEntity2D const & otherEntity ) const
{
    return math::is_intersection( this->get_polygon(),
                                  otherEntity.get_polygon() );
}

void StaticEntity2D::render_before( Render & render ) const
{
    this->custom_draw( render );
}

Entity2D::Entity2D( math::PolygonF quadrangle,
                    std::vector< StaticEntity2D > const & collisionMap,
                    View const & view, input::S_KeyboardMove movementKey )
  : StaticEntity2D( quadrangle ),
    m_collisionMap( collisionMap ),
    m_view( view ),
    m_movementKey( movementKey ),
    m_speed()
{
    this->set_speed( 500.f );
}

math::Vector2F Entity2D::get_speed() const
{
    return m_speed;
}

void Entity2D::set_speed( float speed )
{
    this->m_speed = { speed, speed };
}
void Entity2D::set_speed( math::Vector2F speed )
{
    this->m_speed = speed;
}

bool Entity2D::is_collision_detected() const
{
    for ( StaticEntity2D const & entity : m_collisionMap )
    {
        if ( this->is_intersected_by( entity ) )
        {
            return true;
        }
    }
    return false;
}

void Entity2D::update_before( float deltaTime )
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