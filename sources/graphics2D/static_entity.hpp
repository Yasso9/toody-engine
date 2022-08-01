#pragma once

#include <SFML/Graphics/CircleShape.hpp> // for CircleShape

#include "graphics2D/component.hpp"   // for Component2D
#include "graphics2D/shape.hpp"       // for Shape2D
#include "maths/geometry/polygon.hpp" // for PolygonF

class Render;

class StaticEntity2D : public Component2D,
                       public Shape2D
{
    int m_ID;
    bool m_isCustomisable;
    bool m_editPoint;
    sf::CircleShape m_pointShape;

  public:
    explicit StaticEntity2D( math::PolygonF polygon );
    ~StaticEntity2D() = default;

    void update_before( float deltaTime ) override;

  protected:
    void render_before( Render & render ) const override;

  public:
    void set_customisation( bool isCustomisable );
    bool is_intersected_by( StaticEntity2D const & otherEntity ) const;
};