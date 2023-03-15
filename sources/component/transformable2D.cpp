#include "transformable2D.hpp"

#include "main/render.hpp"

Transformable2D::Transformable2D()
  : m_currentTransformation { sf::Transform::Identity }
{}

void Transformable2D::render_all( RenderContext context ) const
{
    /// @todo review this
    // context.set_transformation( m_currentTransformation );
    Component::render_all( context );
}

void Transformable2D::set_next_transformation( sf::Transform transformation )
{
    m_currentTransformation = transformation;
}