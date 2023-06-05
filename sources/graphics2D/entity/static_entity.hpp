#pragma once

#include <SFML/Graphics/CircleShape.hpp>  // for CircleShape
#include <SFML/Graphics/Texture.hpp>

#include "application/components/component.hpp"      // for Component
#include "graphics2D/surface/shape.hpp"  // for Shape2D
#include "maths/geometry/polygon.hpp"    // for PolygonF

class Render;

class StaticEntity2D : public Component,
                       public Shape2D
{
    int             m_ID;
    bool            m_isCustomisable;
    bool            m_editPoint;
    sf::CircleShape m_pointShape;

  public:
    explicit StaticEntity2D( math::PolygonF polygon );
    explicit StaticEntity2D( sf::Texture const & texture );
    ~StaticEntity2D() = default;

  protected:
    void update ( UpdateContext & context ) override;
    void render ( RenderContext & context ) const override;

  public:
    void set_customisation ( bool isCustomisable );
    bool is_intersected_by ( StaticEntity2D const & otherEntity ) const;
};
