#pragma once

#include "graphics2D/component.hpp"
#include "graphics2D/drawable/sprite.hpp"
#include "graphics2D/entity/controllable_entity.hpp"

class CharacterEntity : public Component,
                        public sf::Transformable
{
    Sprite m_sprite;

    Control m_control;

    float m_speed;

  public:
    CharacterEntity( sf::Texture const & texture, Control control );
    virtual ~CharacterEntity() = default;

  private:
    void update ( float deltaTime ) override;
    void render ( Render & render ) const override;
};