#pragma once

#include "graphics2D/sfml.hpp"
#include "main/window.hpp"

class Component2D;
class Component3D;

class Render
{
    sf::RenderTarget & m_target;
    sf::RenderStates m_states;

  public:
    Render( sf::RenderTarget  & target,
            sf::RenderStates states = sf::RenderStates::Default );

    sf::RenderTarget const & get_target() const;
    sf::RenderTarget & get_target();
    sf::RenderStates get_state() const;
    void set_texture( sf::Texture const & texture );

    // void draw( Component2D const & component ) const;
    // void draw( Component3D const & component ) const;
    // template < C_IsDrawable DrawableClass >
    // void draw( DrawableClass const & sfmlDrawable ) const;
};

// template < C_IsDrawable DrawableClass >
// void Render::draw( DrawableClass const & sfmlDrawable ) const
// {
//     m_target.draw( sfmlDrawable, m_states );
// }