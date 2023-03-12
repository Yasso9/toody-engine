#pragma once

#include "main/window.hpp"

class UpdateContext
{
  public:
    Window const & window;
    float const    deltaTime;

    UpdateContext( Window const & window, float deltaTime );
};

class RenderContext
{
  public:
    Window &         window;
    sf::RenderStates renderStates;

    RenderContext( Window & window );

    void set_texture ( sf::Texture const & texture );
    void set_transformation ( sf::Transform transformation );

    void draw ( sf::Drawable const & drawable ) const;
    void draw ( std::vector< sf::Vertex > const & vertices,
                sf::PrimitiveType                 primitive ) const;
};