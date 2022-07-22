#include "object.hpp"

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

StaticEntity2D::StaticEntity2D( math::PolygonF polygon ) : m_shape( polygon )
{
    this->m_shape.setFillColor( sf::Color::Red );
}

math::PointF StaticEntity2D::get_position() const
{
    return math::PointF { this->getPosition() };
}

math::PolygonF StaticEntity2D::get_polygon( bool isSizeAdded ) const
{
    math::PolygonF polygonToReturn { m_shape.polygon };

    if ( isSizeAdded )
    {
        polygonToReturn.move( this->getPosition() );
    }

    return polygonToReturn;
}

void StaticEntity2D::set_polygon( math::PolygonF polygon )
{
    m_shape.polygon = polygon;
}

bool StaticEntity2D::is_intersected_by(
    StaticEntity2D const & otherEntity ) const
{
    return math::is_intersection( this->get_polygon(),
                                  otherEntity.get_polygon() );
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

void Entity2D::update_extra( float deltaTime )
{
    math::Vector2F moveSpeed { ( m_speed / m_view.get_zoom() ) * deltaTime };
    /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
    /// @todo mettre ça dans le process event
    math::Vector2F moveDirection { input::get_movement_vector(
        m_movementKey ) };

    this->move( moveSpeed * moveDirection );

    if ( this->is_collision_detected() )
    {
        this->move( -( moveSpeed * moveDirection ) );
    }
}