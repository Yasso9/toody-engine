#pragma once

#include <functional>  // for function

#include "main/window.hpp"  // for Window

class UpdateContext
{
  public:
    Window &    window;
    float const deltaTime;

    UpdateContext( Window & aWindow, float aDeltaTime );
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