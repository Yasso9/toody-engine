#include "player.hpp"

#include <cassert>
#include <cmath>
#include <exception>
#include <stdexcept>

Player::Player( sf::Texture const & texture ) : Entity( texture )
{
    // Initial state of the player
    this->m_direction = E_Direction::Down;
    this->m_state = Player::E_State::Normal;

    this->m_spritePixelSize = 30.f;

    sf::Vector2u const textureSize { this->m_texture.getSize() };

    float const numberOfRow { textureSize.x / this->m_spritePixelSize };
    float const numberOfLine { textureSize.y / this->m_spritePixelSize };
    if ( std::trunc( numberOfRow ) != numberOfRow
         || std::trunc( numberOfLine ) != numberOfLine )
    {
        throw std::runtime_error {
            "Size of the texture rect not correctly set"
        };
    }

    this->m_stateNumber.insert( { Player::E_State::Normal,
                                  { { E_Direction::Up, { 0u } },
                                    { E_Direction::Down, { 0u } },
                                    { E_Direction::Right, { 0u } },
                                    { E_Direction::Left, { 0u } } } } );
    this->m_stateNumber.insert( { Player::E_State::Walking,
                                  { { E_Direction::Up, { 0u } },
                                    { E_Direction::Down, { 0u } },
                                    { E_Direction::Right, { 0u } },
                                    { E_Direction::Left, { 0u } } } } );
    this->m_stateNumber.insert( { Player::E_State::Running,
                                  { { E_Direction::Up, { 0u } },
                                    { E_Direction::Right, { 0u } },
                                    { E_Direction::Down, { 0u } },
                                    { E_Direction::Left, { 0u } } } } );

    // float const maxSprite { numberOfRow * numberOfLine };
}

void Player::set_direction( E_Direction const & direction )
{
    this->m_direction = direction;
}

void Player::set_state( Player::E_State const & playerState )
{
    this->m_state = playerState;
}

void Player::update()
{
    float speedVariation { 0.f };
    switch ( this->m_state )
    {
    case Player::E_State::Normal :
        speedVariation = 1.f;
        break;
    case Player::E_State::Running :
        speedVariation = 2.f;
        break;
    default :
        speedVariation = 0.f;
        break;
    }

    this->move( this->get_movement( this->m_direction ) * this->m_speed
                * speedVariation );

    // The origin must be set to the center of the sprite
    // this->m_sprite.setOrigin();

    // // We choose the proper texture rect of the texture that we had loaded
    // this->m_sprite.setTextureRect();
}

sf::Vector2f Player::get_movement( E_Direction const direction ) const
{
    sf::Vector2f movementDistance { 0.f, 0.f };
    float const distance { 1.f };

    switch ( direction )
    {
    case E_Direction::Up :
        movementDistance.y = -distance;
        break;
    case E_Direction::Down :
        movementDistance.y = distance;
        break;
    case E_Direction::Right :
        movementDistance.x = distance;
        break;
    case E_Direction::Left :
        movementDistance.x = -distance;
        break;
    default :
        assert( false && "Unknown Direction" );
        break;
    }

    return movementDistance;
}
