#pragma once

#include <SFML/Graphics/Transformable.hpp>  // for Transformable

#include "component/component.hpp"  // for Component

class Transformable2D : public Component,
                        public sf::Transformable
{
    sf::Transform m_currentTransformation;

  protected:
    Transformable2D();
    virtual ~Transformable2D() = default;

  public:
    virtual void render_all ( RenderContext context ) const override;

  protected:
    /// @todo se demander si on ne supprimer pas cette fonction
    void set_next_transformation ( sf::Transform transformation );
};