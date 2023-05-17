#include "contexts.hpp"

UpdateContext::UpdateContext( Window & aWindow, float aDeltaTime )
  : window { aWindow }, deltaTime { aDeltaTime }
{}

RenderContext::RenderContext( Window & aWindow )
  : window { aWindow }, renderStates { sf::RenderStates::Default }
{}

void RenderContext::set_texture( sf::Texture const & texture )
{
    renderStates.texture = &texture;
}

void RenderContext::set_transformation( sf::Transform transformation )
{
    renderStates.transform = transformation;
}

void RenderContext::draw( sf::Drawable const & drawable ) const
{
    window.draw( drawable, renderStates );
}

void RenderContext::draw( std::vector< sf::Vertex > const & vertices,
                          sf::PrimitiveType                 primitive ) const
{
    window.draw( &vertices[0], vertices.size(), primitive, renderStates );
}