#pragma once

#include <map>     // for map
#include <string>  // for string
#include <vector>  // for vector

#include <SFML/Graphics/Rect.hpp>    // for IntRect
#include <SFML/Graphics/Sprite.hpp>  // for Sprite
#include <SFML/System/Vector2.hpp>   // for Vector2f, Vector2u

#include "application/components/transformable2D.hpp"     // for TransformableComponent
#include "graphics2D/cardinal_direction.hpp"  // for E_Direction

class Render;

namespace sf
{
    class Texture;
}  // namespace sf

class Player : public Transformable2D
{
  public:
    /// @brief States that the player can have
    enum class E_State
    {
        Normal = 0,
        Walking,
        Running,
    };

    struct CompleteState
    {
        Player::E_State state;
        E_Direction     direction;
    };

  private:
    sf::Texture const & m_texture;
    sf::Sprite          m_sprite;

    std::string m_name;
    float       m_speed;

    Player::E_State m_state;
    E_Direction     m_direction;

    float m_deltaTime;

    /// @brief Length in pixel of each cell of the texture sprite
    sf::Vector2f m_spritePixelSize;
    /// @brief Number of cells of the texture sprite
    sf::Vector2u m_spriteNumberOfCells;
    /// @brief Number of the different sprites
    ///        of this playerState according to the state
    std::map< Player::E_State,
              std::map< E_Direction, std::vector< unsigned int > > >
        m_spriteValue;
    /// @brief Last state and direction of the player
    CompleteState m_lastState;
    /// @brief Index of the last sprite for the animation
    unsigned int m_lastSpriteIndex;
    /// @brief Time elapsed since last frame
    float m_timeElapsed;

  public:
    Player();
    virtual ~Player() = default;

    void set_direction ( E_Direction const & direction );
    void set_state ( Player::E_State const & playerState );

    void update_before ( UpdateContext & context ) override;

  private:
    void render_before ( RenderContext & context ) const override;

    void init_sprite_value ();
    void init_sprite_number_of_cells ();
    void reset_last_sprite_index ();

    void update_events ();

    /// @brief return the movement of the direction
    sf::Vector2f get_movement () const;

    /// @brief return the sprite number according to the current
    ///        direction and state of the player
    unsigned int get_current_sprite_number ();

    /// @brief return the texture rect according to the current
    ///        sprite number of the player
    sf::IntRect get_current_texture_rect ();

    void update_delta_time ( float const & deltaTime );
    void update_movement ();
    void update_texture_rect ();
};
