#pragma once

#include <SFML/Graphics/RenderStates.hpp>  // for RenderStates, RenderStates...

namespace sf
{
    class RenderTarget;
    class Texture;
}  // namespace sf

class Render
{
    sf::RenderTarget & m_target;
    sf::RenderStates   m_states;

  public:
    Render(
        sf::RenderTarget & target,
        sf::RenderStates   states = sf::RenderStates::Default );

    sf::RenderTarget & get_target ();
    sf::RenderStates   get_state () const;
    void               set_texture ( sf::Texture const & texture );
};

// template < C_IsDrawable DrawableClass >
// void Render::draw( DrawableClass const & sfmlDrawable ) const
// {
//     m_target.draw( sfmlDrawable, m_states );
// }