#pragma once

#include <SFML/Graphics/Transformable.hpp>  // for Transformable

#include "application/components/component.hpp"  // for Component

class Transformable2D : public Component,
                        public sf::Transformable
{
    sf::Transform m_currentTransformation;

  protected:
    Transformable2D();
    virtual ~Transformable2D() = default;

  public:
    virtual void render_all ( RenderContext & context ) const override;

  protected:
    void set_next_transformation ( sf::Transform transformation );
};
