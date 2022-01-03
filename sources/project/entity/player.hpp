#pragma once

#include <project/entity/entity.hpp>

struct SpriteTextureRect
{
    SpriteTextureRect( unsigned int v_spriteNumber,
                       std::map<Direction, sf::Rect<float>> v_spritePositions )
      : spriteNumber( v_spriteNumber ), spritePositions( v_spritePositions )
    {
    }

    unsigned int spriteNumber;
    /** @brief Initial positions of the different sprite
     * of this playerState according to the direction
     */
    std::map<Direction, sf::Rect<float>> spritePositions;
};

class Player final : public Entity
{
  public:
    Player( sf::Texture const & texture );

    void walk( Direction const & direction );
    void run( Direction const & direction );
    void update();

  private:
    PlayerState m_state {};
    Direction m_direction {};

    std::map<PlayerState, SpriteTextureRect> m_spriteTextureRect {};
};