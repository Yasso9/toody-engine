#include "basic_entity.hpp"

BasicEntity2D::BasicEntity2D( math::PolygonF quadrangle, View const & view )
  : StaticEntity2D { quadrangle }, m_view { view }, m_speed {}
{
    this->set_speed( 500.f );
}

math::Vector2F BasicEntity2D::get_speed() const
{
    return m_speed;
}

void BasicEntity2D::set_speed( float speed )
{
    this->set_speed( { speed, speed } );
}

void BasicEntity2D::set_speed( math::Vector2F speed )
{
    m_speed = speed;
}

void BasicEntity2D::update( UpdateContext /* context */ ) {}