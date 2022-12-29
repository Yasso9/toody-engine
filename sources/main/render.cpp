#include "render.hpp"

#include "graphics2D/view.hpp"

namespace sf
{
    class RenderTarget;
    class Texture;
}  // namespace sf

Render::Render( sf::RenderTarget & target, sf::RenderStates states )
  : m_target { target }, m_renderStates { states }
{}

sf::RenderStates Render::get_render_states() const
{
    return m_renderStates;
}

sf::RenderTarget & Render::get_target()
{
    return m_target;
}

void Render::set_texture( sf::Texture const & texture )
{
    m_renderStates.texture = &texture;
}

void Render::set_transformation( sf::Transform transformation )
{
    m_renderStates.transform = transformation;
}

void Render::set_view( View const & view )
{
    m_target.setView( view );
}

void Render::reset_view()
{
    this->set_view( m_target.getDefaultView() );
}

void Render::draw( sf::Drawable const & drawable ) const
{
    m_target.draw( drawable, m_renderStates );
}

void Render::draw( std::vector< sf::Vertex > const & vertices,
                   sf::PrimitiveType                 primitive ) const
{
    m_target.draw( &vertices[0], vertices.size(), primitive, m_renderStates );
}