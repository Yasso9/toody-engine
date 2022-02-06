#pragma once

#include "entity/player.hpp"
#include "tools/geometry.hpp"
#include "tools/sfml.hpp"

class TextureSprite
{
  public:
    TextureSprite();

    sf::Vector2f get_size( Player::E_State const & state,
                           E_Direction const & direction ) const;

    void change( sf::Sprite & sprite, Player::E_State const & state,
                 E_Direction const & direction );

  private:
    std::map<Player::E_State, std::vector<sf::IntRect>> m_rectSprite {};

    std::map<Player::E_State, unsigned int> m_nbImage {};

    int m_actualState {};
    int m_lastState {};
};