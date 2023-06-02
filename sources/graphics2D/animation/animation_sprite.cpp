#include "animation_sprite.hpp"

#include <iostream>

AnimationSprite::AnimationSprite( T_StateMap animations, std::string state,
                                  E_Direction direction )
  : m_animations { animations },
    m_isRunning { false },
    m_state { state },
    m_direction { direction },
    m_animationIndex { 0u },
    m_timePerFrame { 0.2f },
    m_elapsedTime { 0.f }
{}

void AnimationSprite::update( UpdateContext & context )
{
    m_elapsedTime += context.deltaTime;

    if ( m_elapsedTime >= m_timePerFrame )
    {
        m_elapsedTime = 0.f;
        this->increment_animation_index();
    }
}

E_Direction AnimationSprite::get_direction() const
{
    return m_direction;
}

std::string AnimationSprite::get_state() const
{
    return m_state;
}

std::vector< unsigned int > AnimationSprite::get_current_sprite_list() const
{
    return m_animations.at( m_state ).at( m_direction );
}

unsigned int AnimationSprite::get_current_sprite() const
{
    return this->get_current_sprite_list()[m_animationIndex];
}

void AnimationSprite::set_direction( E_Direction direction )
{
    if ( m_direction == direction )
    {
        return;
    }

    m_direction = direction;
    this->reset_animation();
}

void AnimationSprite::set_direction( math::Vector2F movement )
{
    if ( movement.x == 0.f && movement.y == 0.f )
    {
        return;
    }

    E_Direction direction { this->get_direction() };

    if ( movement.x >= 0.f && movement.y >= 0.f )
    {
        direction = movement.x > movement.y ? E_Direction::Right
                                            : E_Direction::Down;
    }
    else if ( movement.x >= 0.f && movement.y <= 0.f )
    {
        direction = movement.x > -movement.y ? E_Direction::Right
                                             : E_Direction::Up;
    }
    else if ( movement.x <= 0.f && movement.y >= 0.f )
    {
        direction = movement.x < -movement.y ? E_Direction::Left
                                             : E_Direction::Down;
    }
    else if ( movement.x <= 0.f && movement.y <= 0.f )
    {
        direction = movement.x < movement.y ? E_Direction::Left
                                            : E_Direction::Up;
    }

    this->set_direction( direction );
}

void AnimationSprite::set_state( std::string state )
{
    if ( m_state == state )
    {
        return;
    }

    m_state = state;
    this->reset_animation();
}

void AnimationSprite::add_state( std::string    state,
                                 T_DirectionMap directionAnimation )
{
    m_animations.insert( { state, directionAnimation } );
}

void AnimationSprite::increment_animation_index()
{
    ++m_animationIndex;

    if ( m_animationIndex >= this->get_current_sprite_list().size() )
    {
        m_animationIndex = 0u;
    }
}

void AnimationSprite::reset_animation()
{
    m_animationIndex = 0u;
    m_elapsedTime    = 0.f;
}
