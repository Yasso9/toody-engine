#pragma once

#include <SFML/Graphics/Sprite.hpp>  // for Sprite

#include "graphics2D/entity/controllable_entity.hpp"

class CharacterEntity : public Component2D,
                        public sf::Transformable
{
    sf::Texture const & m_texture;
    sf::Sprite          m_sprite;

  public:
    CharacterEntity( sf::Texture const & texture );
    virtual ~CharacterEntity() = default;

  private:
    void update ( float deltaTime ) override;
    void render ( Render & render ) const override;
};