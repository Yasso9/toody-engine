#pragma once

#include <cstddef>  // for size_t

#include <SFML/Graphics/Shape.hpp>  // for Shape
#include <SFML/System/Vector2.hpp>  // for Vector2f

#include "components/contexts.hpp"
#include "maths/geometry/point.hpp"    // for PointF
#include "maths/geometry/polygon.hpp"  // for PolygonF
#include "maths/vector2.hpp"           // for Vector2F

class Render;

class Shape2D : public sf::Shape
{
    math::PolygonF m_polygon;

  public:
    Shape2D( math::PolygonF polygon );

    unsigned int   get_point_count () const;
    math::Vector2F get_point ( unsigned int index ) const;

    math::PointF get_position () const;

    math::PolygonF get_polygon ( bool isSizeAdded = true ) const;
    void           set_polygon ( math::PolygonF polygon );

  protected:
    void custom_draw ( RenderContext context ) const;

  private:
    std::size_t  getPointCount () const override;
    sf::Vector2f getPoint ( std::size_t index ) const override;

    using sf::Shape::getPosition;
    using sf::Shape::update;
};