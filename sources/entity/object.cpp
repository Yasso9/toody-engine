#include "object.hpp"

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

StaticEntity2D::StaticEntity2D( math::PolygonF polygon ) : m_polygon()
{
    this->set_polygon( polygon );
    this->setFillColor( sf::Color::Red );
}

math::PointF StaticEntity2D::get_position() const
{
    return math::PointF { this->getPosition() };
}

math::PolygonF StaticEntity2D::get_polygon( bool /* getSize */ ) const
{
    /// @todo prendre en compte le get size
    return m_polygon;

    // math::PolygonF quadrangle {};
    // for ( unsigned int i_point = 0u; i_point < this->getPointCount();
    //       ++i_point )
    // {
    //     quadrangle[i_point] = this->getPoint( i_point );
    //     if ( getSize )
    //     {
    //         quadrangle[i_point] += this->get_position();
    //     }
    // }

    // return quadrangle;
}

void StaticEntity2D::set_polygon( math::PolygonF polygon )
{
    m_polygon = polygon;
    // this->setPointCount( polygon.get_number_of_points() );

    // this->setPosition( 0.f, 0.f );
    // if ( polygon[0] == math::Vector2F { 0.f, 0.f } )
    // {
    //     this->setPosition( polygon[0] );
    // }

    // for ( unsigned int i_point = 0u; i_point < this->getPointCount();
    //       ++i_point )
    // {
    //     this->setPoint( i_point, polygon[i_point] - this->get_position() );
    // }
}

bool StaticEntity2D::is_intersected_by(
    StaticEntity2D const & otherEntity ) const
{
    return math::is_intersection( this->get_polygon(),
                                  otherEntity.get_polygon() );
}

Entity2D::Entity2D( math::PolygonF quadrangle,
                    T_CollisionMap const & collisionMap,
                    keyboard_move::S_Key movementKey )
  : StaticEntity2D( quadrangle ),
    m_collisionMap( collisionMap ),
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

void Entity2D::update( float deltaTime, math::Vector2F viewZoom )
{
    math::Vector2F moveSpeed { ( m_speed / viewZoom ) * deltaTime };
    /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
    math::Vector2F moveDirection { keyboard_move::get_vector( m_movementKey ) };

    this->move( moveSpeed * moveDirection );

    if ( this->is_collision_detected() )
    {
        this->move( -( moveSpeed * moveDirection ) );
    }
}