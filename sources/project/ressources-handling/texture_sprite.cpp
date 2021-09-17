#include "texture_sprite.hpp"

TextureSprite::TextureSprite()
{
    this->m_rectSprite = {
        {
            PlayerState::Normal, {
            sf::IntRect( 156, 25, 21, 28 ),
            sf::IntRect( 8, 25, 21, 28 ),
            sf::IntRect( 81, 25, 22, 28 ),
            sf::IntRect( 230, 25, 22, 38 )
        }},
        {
            PlayerState::Walking, {
            sf::IntRect( 156, 25, 21, 28 ),
            sf::IntRect( 8, 25, 21, 28 ),
            sf::IntRect( 81, 25, 22, 28 ),
            sf::IntRect( 230, 25, 22, 38 )
        }},
        {
            PlayerState::Running, {
            sf::IntRect( 165, 71, 22, 28 ),
            sf::IntRect( 6, 74, 24, 26 ),
            sf::IntRect( 88, 73, 22, 26 ),
            sf::IntRect( 240, 74, 23, 25 )
        }},
    };

    this->m_nbImage = {
        { PlayerState::Normal, 1 },
        { PlayerState::Walking, 3 },
        { PlayerState::Running, 3 },
    };

    this->m_actualState = 0;
    this->m_lastState = 2;
}

sf::Vector2f TextureSprite::get_size( PlayerState const & state,
    Direction const & direction ) const
{
    return sf::Vector2f(
        static_cast<float>(this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].width),
        static_cast<float>(this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].height)
    );
}

void TextureSprite::change( sf::Sprite & sprite, PlayerState const & state,
    Direction const & direction )
{
    // Fonctionne que pour 3 image
    if ( this->m_actualState == 0 && this->m_lastState == 2 )
    {
        this->m_lastState = 0; this->m_actualState = 1;
    }
    else if ( this->m_actualState == 0 && this->m_lastState == 1 )
    {
        this->m_lastState = 0; this->m_actualState = 2;
    }
    else if ( this->m_actualState == 1 || this->m_actualState == 2 )
    {
        this->m_lastState = this->m_actualState;
        this->m_actualState = 0;
    }

    if ( this->m_nbImage.at( state ) == 1 )
    {
        this->m_lastState = 2; this->m_actualState = 0;
    }

    this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].left = (
        this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].left +
        this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].width * this->m_actualState
    );

    sprite.setOrigin(
        static_cast<float>(this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].width) / 2,
        static_cast<float>(this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)].height) / 2
    );

    sprite.setTextureRect( this->m_rectSprite.at( state )[static_cast<std::size_t>(direction)] );
}