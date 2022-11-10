#pragma once

#include "component/transformable2D.hpp"
#include "graphics2D/drawable/sprite.hpp"
#include "graphics2D/entity/controllable_entity.hpp"

class CharacterEntity : public Transformable2D
{
    Sprite m_sprite;

    Control m_control;

    float m_speed;

  public:
    CharacterEntity( sf::Texture const & texture, Control control );
    virtual ~CharacterEntity() = default;

  private:
    void update ( float deltaTime ) override;
};