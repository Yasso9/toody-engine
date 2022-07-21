#include "render.hpp"

#include "graphics2D/component.hpp"

Render::Render( Window const & window, sf::RenderTarget & target,
                sf::RenderStates states )
  : m_window( window ), m_target( target ), m_states( states )
{}

Window const & Render::get_window() const
{
    return m_window;
}

sf::RenderTarget const & Render::get_target() const
{
    return m_target;
}
sf::RenderTarget & Render::get_target()
{
    Render const * thisConst { this };
    return const_cast< sf::RenderTarget & >( thisConst->get_target() );
}

void Render::set_texture( sf::Texture const & texture )
{
    m_states.texture = &texture;
}

void Render::draw( Component2D const & component ) const
{
    m_target.draw( component, m_states );
}
void Render::draw( Component3D const & component ) const
{
    m_window.draw( component );
}