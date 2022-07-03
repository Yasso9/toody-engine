#pragma once

#include "graphics2D/sfml.hpp"
#include "input/movement_key.hpp"
#include "maths/maths.hpp"

static constexpr unsigned int NUMBER_OF_POINT { 4u };

/// @todo create a base class StaticEntity
class Entity2D : public sf::ConvexShape
{
    keyboard_move::S_Key m_movementKey;
    math::Vector2F m_speed;

  public:
    Entity2D( keyboard_move::S_Key movementKey )
      : m_movementKey( movementKey ), m_speed()
    {
        this->set_speed( 500.f );

        this->setPointCount( NUMBER_OF_POINT );
        this->set_polygon( math::QuadrangleF {} );
        this->setFillColor( sf::Color::White );
        this->setOutlineColor( sf::Color::Black );
    }

    math::Vector2F get_speed() const { return m_speed; }

    void set_speed( float speed ) { this->m_speed = { speed, speed }; }
    void set_speed( math::Vector2F speed ) { this->m_speed = speed; }

    void update( float deltaTime,
                 math::Vector2F viewZoom = math::Vector2F( 1.f, 1.f ) )
    {
        math::Vector2F const moveSpeed { ( m_speed / viewZoom ) * deltaTime };
        /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
        math::Vector2F moveDirection { keyboard_move::get_vector(
            m_movementKey ) };

        this->move( moveSpeed * moveDirection );
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
                quadrangle[i_point] += math::PointF { this->getPosition() };
            }
        }

        return quadrangle;
    }

    void set_polygon( math::QuadrangleF quadrangle )
    {
        for ( unsigned int i_point = 0u; i_point < this->getPointCount();
              ++i_point )
        {
            this->setPoint( i_point, quadrangle[i_point] );
        }
    }

    bool is_intersected_by( Entity2D const & otherEntity ) const
    {
        return math::is_intersection( this->get_polygon(),
                                      otherEntity.get_polygon() );
    }
};