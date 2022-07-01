#pragma once

#include "maths/maths.hpp"

#include "graphics2D/sfml.hpp"

static constexpr unsigned int NUMBER_OF_POINT { 4u };

class Entity2D : public sf::ConvexShape
{
  public:
    Entity2D()
    {
        this->setPointCount( NUMBER_OF_POINT );
        this->set_quadrangle( math::QuadrangleF {} );
        this->setFillColor( sf::Color::Black );
        // this->setOutlineColor( sf::Color::White );
    }

    math::QuadrangleF get_quadrangle() const
    {
        math::QuadrangleF quadrangle {};
        for ( unsigned int i_point = 0u; i_point < this->getPointCount();
              ++i_point )
        {
            quadrangle[i_point] = this->getPoint( i_point );
        }

        return quadrangle;
    }

    void set_quadrangle( math::QuadrangleF quadrangle )
    {
        for ( unsigned int i_point = 0u; i_point < this->getPointCount();
              ++i_point )
        {
            this->setPoint( i_point, quadrangle[i_point] );
        }
    }

    bool is_intersected_by( Entity2D const & otherEntity ) const
    {
        return math::is_intersection( this->get_quadrangle(),
                                      otherEntity.get_quadrangle() );
    }
};