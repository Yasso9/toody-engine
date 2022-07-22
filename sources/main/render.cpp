#include "render.hpp"

#include "graphics2D/component.hpp"

Render::Render( sf::RenderTarget & target, sf::RenderStates states )
  : m_target( target ), m_states( states )
{}

sf::RenderStates Render::get_state() const
{
    return m_states;
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

// void Render::draw( Component2D const & component ) const
// {
//     m_target.draw( component, m_states );
// }
// void Render::draw( Component3D const & component ) const
// {
//     m_window.draw( component, m_target, m_states );
// }