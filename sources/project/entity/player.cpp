#include "player.hpp"

#include <cassert>

namespace
{
    sf::Vector2f get_movement( Direction const direction, float const distance )
    {
        sf::Vector2f movementDistance { 0.f, 0.f };

        switch ( direction )
        {
        case Direction::Up :
            movementDistance.y = -distance;
            break;
        case Direction::Down :
            movementDistance.y = distance;
            break;
        case Direction::Right :
            movementDistance.x = distance;
            break;
        case Direction::Left :
            movementDistance.x = -distance;
            break;
        default :
            assert( false && "Unknown Direction" );
            break;
        }

        return movementDistance;
    }
} // namespace

Player::Player( sf::Texture const & texture ) : Entity( texture )
{
    this->m_direction = Direction::Down;
    this->m_state = PlayerState::Normal;

    this->m_spriteTextureRect = {
        std::make_pair(
            PlayerState::Normal,
            SpriteTextureRect {
                1,
                std::map<Direction, sf::Rect<float>> {
                    std::make_pair(
                        Direction::Down,
                        sf::Rect<float>( 156.f, 25.f, 21.f, 28.f ) ),
                    std::make_pair( Direction::Down,
                                    sf::Rect<float>( 8.f, 25.f, 21.f, 28.f ) ),
                    std::make_pair( Direction::Down,
                                    sf::Rect<float>( 81.f, 25.f, 22.f, 28.f ) ),
                    std::make_pair(
                        Direction::Down,
                        sf::Rect<float>( 230.f, 25.f, 22.f, 38.f ) ) } } ),
        // SpriteTextureRect { PlayerState::Walking,
        //                     { sf::IntRect( 156, 25, 21, 28 ),
        //                       sf::IntRect( 8, 25, 21, 28 ),
        //                       sf::IntRect( 81, 25, 22, 28 ),
        //                       sf::IntRect( 230, 25, 22, 38 ) } },
        // SpriteTextureRect { PlayerState::Running,
        //                     { sf::IntRect( 165, 71, 22, 28 ),
        //                       sf::IntRect( 6, 74, 24, 26 ),
        //                       sf::IntRect( 88, 73, 22, 26 ),
        //                       sf::IntRect( 240, 74, 23, 25 ) } },
    };
}

void Player::walk( Direction const & direction )
{
    this->move( ::get_movement( direction, 1.f ) * this->m_speed );
}

void Player::run( Direction const & direction )
{
    this->move( ::get_movement( direction, 2.f ) * this->m_speed );
}

void Player::update()
{
    this->m_sprite.setOrigin(
        this->m_spriteTextureRect.at( PlayerState::Normal )
                .spritePositions.at( Direction::Down )
                .width
            / 2.f,
        this->m_spriteTextureRect.at( PlayerState::Normal )
                .spritePositions.at( Direction::Down )
                .height
            / 2.f );

    this->m_sprite.setTextureRect( static_cast<sf::IntRect>(
        this->m_spriteTextureRect.at( PlayerState::Normal )
            .spritePositions.at( Direction::Down ) ) );
}