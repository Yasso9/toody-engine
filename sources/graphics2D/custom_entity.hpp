#pragma once

#include "entity/object.hpp"
#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

/// @brief Entity that can be customised
class CustomEntity2D : public StaticEntity2D
{
    bool m_showCustomisation;
    bool m_editPoint;
    sf::CircleShape m_pointShape;

  public:
    CustomEntity2D( math::PolygonF quadrangle );

    void update_before( float deltaTime ) override;

  private:
    void render_before( Render & render ) const override;
};