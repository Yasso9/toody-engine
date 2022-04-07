#pragma once

#include "graphics/sfml.hpp"

class Entity : public sf::Drawable,
               public sf::Transformable
{
  public:
    Entity( sf::Texture const & texture );
    virtual ~Entity() = default;

  protected:
    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const;

    sf::Sprite m_sprite;
    sf::Texture const m_texture;

    std::string m_name;
    float m_speed;
};