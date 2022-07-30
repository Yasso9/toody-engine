#pragma once

#include "graphics2D/component.hpp"
#include "graphics2D/shape.hpp"
#include "graphics2D/view.hpp"
#include "input/input.hpp"

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