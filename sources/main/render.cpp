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
    this->get_target().setView( view );
}

void Render::reset_view()
{
    this->set_view( this->get_target().getDefaultView() );
}

void Render::draw( sf::Drawable const & drawable ) const
{
    m_target.draw( drawable, m_renderStates );
}