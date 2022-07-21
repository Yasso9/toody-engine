#pragma once

#include "graphics2D/sfml.hpp"
#include "main/window.hpp"

class Component2D;
class Component3D;

class Render
{
    Window const & m_window;
    sf::RenderTarget & m_target;
    sf::RenderStates m_states;

  public:
    Render( Window const & window, sf::RenderTarget & target,
            sf::RenderStates states );

    Window const & get_window() const;
    sf::RenderTarget const & get_target() const;
    sf::RenderTarget & get_target();
    void set_texture( sf::Texture const & texture );

    void draw( Component2D const & component ) const;
    void draw( Component3D const & component ) const;
    template < C_IsDrawable DrawableClass >
    void draw( DrawableClass const & sfmlDrawable ) const;
};

template < C_IsDrawable DrawableClass >
void Render::draw( DrawableClass const & sfmlDrawable ) const
{
    m_target.draw( sfmlDrawable, m_states );
}