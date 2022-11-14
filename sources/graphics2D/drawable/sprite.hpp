#pragma once

#include <SFML/Graphics/Sprite.hpp>  // for Sprite

#include "component/component.hpp"
#include "maths/geometry/polygon.hpp"

class Sprite : public sf::Sprite,
               public Component
{
    math::Vector2F m_size;

  public:
    Sprite( sf::Texture const & texture, math::Vector2F size );
    virtual ~Sprite() = default;

    void render ( Render & render ) const override;

    math::PolygonF get_polygon () const;

    void select_animation ( unsigned int index );

  private:
};