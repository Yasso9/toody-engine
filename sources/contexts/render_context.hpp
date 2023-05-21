#pragma once

#include "contexts/base_context.hpp"  // for BaseContext

class RenderContext : virtual public BaseContext
{
  public:
    sf::RenderStates renderStates;

    RenderContext();
    virtual ~RenderContext() = default;

    void set_texture ( sf::Texture const & texture );
    void set_transformation ( sf::Transform transformation );

    void draw ( sf::Drawable const & drawable );
    void draw ( std::vector< sf::Vertex > const & vertices,
                sf::PrimitiveType                 primitive );
};
