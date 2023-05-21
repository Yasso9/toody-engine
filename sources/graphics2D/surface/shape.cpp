#include "shape.hpp"

#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget

#include "maths/geometry/point.tpp"    // for Point::Point<Type>
#include "maths/geometry/polygon.tpp"  // for Polygon::operator[], Polyg...
#include "maths/vector2.tpp"           // for Vector2::Vector2<Type>

Shape2D::Shape2D( math::PolygonF polygon ) : m_polygon {}
{
    this->set_polygon( polygon );
}

unsigned int Shape2D::get_point_count() const
{
    return m_polygon.get_number_of_points();
};

math::Vector2F Shape2D::get_point( unsigned int index ) const
{
    return m_polygon[index];
};

math::PointF Shape2D::get_position() const
{
    return math::PointF { this->getPosition() };
}

math::PolygonF Shape2D::get_polygon( bool isSizeAdded ) const
{
    // Copy so we don't modify m_polygon;
    math::PolygonF polygonToReturn { m_polygon };

    if ( isSizeAdded )
    {
        polygonToReturn.move( this->get_position() );
    }

    return polygonToReturn;
}

void Shape2D::set_polygon( math::PolygonF polygon )
{
    m_polygon = polygon;
    this->update();
}

void Shape2D::custom_draw( RenderContext & context ) const
{
    // auto renderStates = render.get_state();
    // renderStates.transform *= this->getTransform();
    context.draw( *dynamic_cast< sf::Shape const * >( this ) );
}

std::size_t Shape2D::getPointCount() const
{
    return static_cast< std::size_t >( this->get_point_count() );
}

sf::Vector2f Shape2D::getPoint( std::size_t index ) const
{
    return this->get_point( static_cast< unsigned int >( index ) );
}
