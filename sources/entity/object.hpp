#pragma once

#include "graphics2D/sfml.hpp"
#include "input/movement_key.hpp"
#include "maths/maths.hpp"

static constexpr unsigned int NUMBER_OF_POINT { 4u };

class StaticEntity2D : public sf::ConvexShape
{
  public:
    StaticEntity2D( math::QuadrangleF polygon )
    {
        this->set_polygon( polygon );
        this->setFillColor( sf::Color::Red );
    }

    math::PointF get_position() const
    {
        return math::PointF { this->getPosition() };
    }

    math::QuadrangleF get_polygon( bool getSize = true ) const
    {
        math::QuadrangleF quadrangle {};
        for ( unsigned int i_point = 0u; i_point < this->getPointCount();
              ++i_point )
        {
            quadrangle[i_point] = this->getPoint( i_point );
            if ( getSize )
            {
                quadrangle[i_point] += this->get_position();
            }
        }

        return quadrangle;
    }

    void set_polygon( math::QuadrangleF polygon )
    {
        this->setPointCount( polygon.get_number_of_point() );

        this->setPosition( 0.f, 0.f );
        if ( polygon[0] == math::Vector2F { 0.f, 0.f } )
        {
            this->setPosition( polygon[0] );
        }

        for ( unsigned int i_point = 0u; i_point < this->getPointCount();
              ++i_point )
        {
            this->setPoint( i_point, polygon[i_point] - this->get_position() );
        }
    }

    bool is_intersected_by( StaticEntity2D const & otherEntity ) const
    {
        return math::is_intersection( this->get_polygon(),
                                      otherEntity.get_polygon() );
    }
};

using T_CollisionMap = std::vector< StaticEntity2D >;

/// @brief Moveable Entity
class Entity2D : public StaticEntity2D
{
    T_CollisionMap const & m_collisionMap;
    keyboard_move::S_Key m_movementKey;
    math::Vector2F m_speed;

  public:
    Entity2D( math::QuadrangleF quadrangle, T_CollisionMap const & collisionMap,
              keyboard_move::S_Key movementKey )
      : StaticEntity2D( quadrangle ),
        m_collisionMap( collisionMap ),
        m_movementKey( movementKey ),
        m_speed()
    {
        this->set_speed( 500.f );
    }

    math::Vector2F get_speed() const { return m_speed; }

    void set_speed( float speed ) { this->m_speed = { speed, speed }; }
    void set_speed( math::Vector2F speed ) { this->m_speed = speed; }

    bool is_collision_detected() const
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

    void update( float deltaTime,
                 math::Vector2F viewZoom = math::Vector2F( 1.f, 1.f ) )
    {
        math::Vector2F moveSpeed { ( m_speed / viewZoom ) * deltaTime };
        /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
        math::Vector2F moveDirection { keyboard_move::get_vector(
            m_movementKey ) };

        this->move( moveSpeed * moveDirection );

        if ( this->is_collision_detected() )
        {
            this->move( -( moveSpeed * moveDirection ) );
        }
    }
};