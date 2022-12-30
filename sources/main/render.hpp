#pragma once

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>  // for RenderStates, RenderStates...
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

class View;

class Render
{
    sf::RenderTarget & m_target;
    sf::RenderStates   m_renderStates;

  public:
    Render( sf::RenderTarget & target,
            sf::RenderStates   states = sf::RenderStates::Default );

    sf::RenderTarget & get_target ();
    sf::RenderStates   get_render_states () const;

    void set_texture ( sf::Texture const & texture );
    void set_transformation ( sf::Transform transformation );
    void set_view ( View const & view );
    void reset_view ();

    void draw ( sf::Drawable const & drawable ) const;
    void draw ( std::vector< sf::Vertex > const & vertices,
                sf::PrimitiveType                 primitive ) const;
};