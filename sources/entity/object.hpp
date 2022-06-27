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
        quadrangle.topLeftPosition     = this->getPoint( 0u );
        quadrangle.topRightPosition    = this->getPoint( 1u );
        quadrangle.bottomRightPosition = this->getPoint( 2u );
        quadrangle.bottomLeftPosition  = this->getPoint( 3u );
        return quadrangle;
    }

    void set_quadrangle( math::QuadrangleF quadrangle )
    {
        this->setPoint( 0u, quadrangle.topLeftPosition );
        this->setPoint( 0u, quadrangle.topRightPosition );
        this->setPoint( 0u, quadrangle.bottomRightPosition );
        this->setPoint( 0u, quadrangle.bottomLeftPosition );
    }

    bool is_intersected_by( Entity2D const & otherEntity ) const
    {
        return math::is_intersection( this->get_quadrangle(),
                                      otherEntity.get_quadrangle() );
    }
};