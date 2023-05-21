#include "render_context.hpp"

RenderContext::RenderContext()
  : BaseContext {}, renderStates { sf::RenderStates::Default }
{}

void RenderContext::set_texture( sf::Texture const & texture )
{
    renderStates.texture = &texture;
}

void RenderContext::set_transformation( sf::Transform transformation )
{
    renderStates.transform = transformation;
}

void RenderContext::draw( sf::Drawable const & drawable )
{
    window.draw( drawable, renderStates );
}

void RenderContext::draw( std::vector< sf::Vertex > const & vertices,
                          sf::PrimitiveType                 primitive )
{
    window.draw( &vertices[0], vertices.size(), primitive, renderStates );
}
