#include "render.hpp"

#include "graphics2D/view.hpp"

namespace sf
{
    class RenderTarget;
    class Texture;
}  // namespace sf

Render::Render( sf::RenderTarget & target, sf::RenderStates states )
  : m_target( target ), m_states( states )
{}

sf::RenderStates Render::get_state() const
{
    return m_states;
}

sf::RenderTarget & Render::get_target()
{
    return m_target;
}

void Render::set_texture( sf::Texture const & texture )
{
    m_states.texture = &texture;
}

void Render::set_view( View const & view )
{
    this->get_target().setView( view );
}

void Render::reset_view()
{
    this->set_view( this->get_target().getDefaultView() );
}
