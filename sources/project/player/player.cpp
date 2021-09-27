#include "player.hpp"

Player::Player()
{
    this->m_sprite.setPosition( 200.f, 200.f );
    this->m_direction = Direction::Down;
    this->m_state     = PlayerState::Normal;
    this->m_name      = "Unkown";
    this->m_speed     = 1.f;
    this->m_textureSprite.change( this->m_sprite, this->m_state, this->m_direction );
}

void Player::create( sf::Texture const & texture )
{
    this->m_sprite.setTexture( texture );
}

sf::Vector2f Player::get_position() const
{
    return this->m_sprite.getPosition();
}

void Player::stay()
{
    this->m_state = PlayerState::Normal;
}

void Player::walk( Direction const & dir )
{
    this->m_direction = dir;
    this->m_state     = PlayerState::Walking;
    this->m_sprite.move( get_movement( this->m_direction, 1.f ) * this->m_speed );
}

void Player::run()
{
    if ( this->m_state == PlayerState::Walking )
    {
        this->m_state = PlayerState::Running;
        this->m_sprite.move( get_movement( this->m_direction, 2.f )
                             * this->m_speed );
    }
}

void Player::update()
{
    this->m_textureSprite.change( this->m_sprite, this->m_state, this->m_direction );
}

void Player::render( sf::RenderWindow & target )
{
    target.draw( this->m_sprite );
}

Player::~Player() {}