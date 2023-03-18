#pragma once

#include <map>
#include <string>

#include "components/component.hpp"
#include "graphics2D/cardinal_direction.hpp"

class AnimationSprite : public Component
{
  public:
    using T_DirectionMap = std::map< E_Direction, std::vector< unsigned int > >;
    using T_StateMap     = std::map< std::string, T_DirectionMap >;

  private:
    T_StateMap   m_animations;
    bool         m_isRunning;
    std::string  m_state;
    E_Direction  m_direction;
    unsigned int m_animationIndex;
    float        m_timePerFrame;
    float        m_elapsedTime;

  public:
    AnimationSprite( T_StateMap animations, std::string state,
                     E_Direction direction );

  private:
    void update ( UpdateContext context ) override;

  public:
    E_Direction                 get_direction () const;
    std::string                 get_state () const;
    std::vector< unsigned int > get_current_sprite_list () const;
    unsigned int                get_current_sprite () const;

    void set_direction ( E_Direction direction );
    void set_direction ( math::Vector2F movement );
    void set_state ( std::string state );

    void add_state ( std::string state, T_DirectionMap directionAnimation );

  private:
    void increment_animation_index ();
    void reset_animation ();
};