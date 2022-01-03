#pragma once

#include <project/utility/utility.hpp>

class TextureSprite
{
  public:
    TextureSprite();

    sf::Vector2f get_size( PlayerState const & state,
                           Direction const & direction ) const;

    void change( sf::Sprite & sprite, PlayerState const & state,
                 Direction const & direction );

  private:
    std::map<PlayerState, std::vector<sf::IntRect>> m_rectSprite {};

    std::map<PlayerState, unsigned int> m_nbImage {};

    int m_actualState {};
    int m_lastState {};
};