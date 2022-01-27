#pragma once

#include <project/entity/entity.hpp>
// #include <project/ressources-handling/texture_sprite.hpp>
#include <project/tools/geometry.hpp>

class Player final : public Entity
{
  public:
    /// @brief States that the player can have
    enum class E_State
    {
        Normal = 0,
        Walking,
        Running,
        Poketech,
        Cycling,
        PokemonCenter,
        Watering,
        Surfing,
        UsingPokemon,
        Fishing,
        Saving,
        EnumLast,
    };

    Player( sf::Texture const & texture );

    void set_direction( E_Direction const & direction );
    void set_state( Player::E_State const & playerState );

    /// @brief Update the position and the shape of the player
    ///        depending of the direction and the state
    void update();

  private:
    /// @brief return the movement of the direction
    sf::Vector2f get_movement( E_Direction const direction ) const;

    Player::E_State m_state;
    E_Direction m_direction;

    /// @brief Number of the different sprite
    ///        of this playerState according to the state
    std::map<Player::E_State, std::map<E_Direction, std::vector<unsigned int>>>
        m_stateNumber;
    /// @brief Length of each box of the texture sprite
    float m_spritePixelSize;
    /// @brief Number of the current sprite that must be printed
    // unsigned int m_spriteNumber;
};