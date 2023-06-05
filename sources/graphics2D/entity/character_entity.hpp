#pragma once

#include <map>

#include "application/components/transformable2D.hpp"
#include "graphics2D/animation/animation_sprite.hpp"
#include "graphics2D/drawable/sprite.hpp"
#include "graphics2D/entity/controllable_entity.hpp"
#include "tools/tools.hpp"

class CharacterEntity : public Transformable2D
{
  private:
    Sprite m_sprite;

    Control m_control;

    float m_speed;

    bool m_isWindowShowed;

    AnimationSprite m_animation;

  public:
    CharacterEntity( sf::Texture const & texture, Control control );
    virtual ~CharacterEntity() = default;

  private:
    void update ( UpdateContext & context ) override;

  public:
    math::PolygonF get_polygon () const;

  private:
    math::Vector2F get_movement ( UpdateContext & context ) const;
};
