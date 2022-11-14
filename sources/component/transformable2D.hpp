#pragma once

#include "component/component.hpp"

class Transformable2D : public Component,
                        public sf::Transformable
{
    sf::Transform m_currentTransformation;

  protected:
    Transformable2D();
    virtual ~Transformable2D() = default;

  public:
    virtual void render_all ( Render & render ) const override;

  protected:
    void set_next_transformation ( sf::Transform transformation );
};