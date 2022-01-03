#pragma once

#include <project/ressources-handling/texture_sprite.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
  public:
    Entity( sf::Texture const & texture );

    Entity( Entity && ) noexcept = delete;
    Entity & operator=( Entity const & ) noexcept = delete;
    Entity & operator=( Entity && ) noexcept = delete;
    virtual ~Entity() noexcept = default;

  protected:
    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const;

    sf::Sprite m_sprite {};
    sf::Texture const m_texture;

    std::string m_name {};
    float m_speed {};
};