#pragma once

#include "graphics2D/sfml.hpp"
#include "main/render.hpp"
#include "maths/maths.hpp"

class Shape2D : public sf::Shape
{
    math::PolygonF m_polygon;

  public:
    Shape2D( math::PolygonF polygon );

    unsigned int get_point_count() const;
    math::Vector2F get_point( unsigned int index ) const;

    math::PointF get_position() const;

    math::PolygonF get_polygon( bool isSizeAdded = true ) const;
    void set_polygon( math::PolygonF polygon );

  protected:
    void custom_draw( Render & render ) const;

  private:
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint( std::size_t index ) const override;

    using sf::Shape::getPosition;
    using sf::Shape::update;
};