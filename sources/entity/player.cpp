#include "player.hpp"

#include <exception>
#include <stdexcept>

#include "input/input.hpp"
#include "main/resources.hpp"
#include "tools/assertion.hpp"
#include "tools/tools.hpp"

Player::Player()
  : m_texture( Resources::get_instance().get_texture(
      Resources::E_TextureKey::Player ) ),
    m_sprite( m_texture ),
    m_name( "Unkown"s ),
    m_speed( 30.f ),
    m_state(),
    m_direction(),
    m_deltaTime(),
    m_spritePixelSize(),
    m_spriteNumberOfCells(),
    m_spriteValue(),
    m_lastState(),
    m_lastSpriteIndex(),
    m_timeElapsed()
{
    // Initial states of the player
    this->m_direction = E_Direction::Down;
    this->m_state     = Player::E_State::Normal;

    // Each cells measure 30 pixels of width and height
    this->m_spritePixelSize = sf::Vector2f { 30.f, 30.f };

    this->m_deltaTime   = 0.f;
    this->m_timeElapsed = 0.f;

    this->reset_last_sprite_index();
    this->init_sprite_number_of_cells();
    this->init_sprite_value();
    // To initialisez the first texture rect
    this->update_texture_rect();

    this->m_lastState = { this->m_state, this->m_direction };
}

void Player::reset_last_sprite_index()
{
    this->m_lastSpriteIndex = 0;
}

void Player::init_sprite_value()
{
    // For each direction of states, we set the corresponding sprite value
    this->m_spriteValue.insert( {
        Player::E_State::Normal,
        {{ E_Direction::Up, { 6u } },
                           { E_Direction::Down, { 0u } },
                           { E_Direction::Right, { 3u } },
                           { E_Direction::Left, { 9u } }}
    } );
    this->m_spriteValue.insert( {
        Player::E_State::Walking,
        {{ E_Direction::Up, { 6u, 7u, 6u, 8u } },
                            { E_Direction::Down, { 0u, 1u, 0u, 2u } },
                            { E_Direction::Right, { 3u, 4u, 3u, 5u } },
                            { E_Direction::Left, { 9u, 10u, 9u, 11u } }}
    } );
    this->m_spriteValue.insert( {
        Player::E_State::Running,
        {{ E_Direction::Up, { 0u } },
                            { E_Direction::Right, { 0u } },
                            { E_Direction::Down, { 0u } },
                            { E_Direction::Left, { 0u } }}
    } );
}

void Player::init_sprite_number_of_cells()
{
    // Get the size of the loaded texture
    sf::Vector2u const textureSize { this->m_texture.getSize() };

    // Get the number of sprite that the texture have
    float const numberOfRow { static_cast< float >( textureSize.x )
                              / this->m_spritePixelSize.x };
    float const numberOfLine { static_cast< float >( textureSize.y )
                               / this->m_spritePixelSize.y };

    if ( tools::is_integer( numberOfRow ) && tools::is_integer( numberOfLine ) )
    {
        throw std::domain_error {
            "Player texture rect not correctly set. The number of pixel per "
            "sprite does not correspond to the size that the texture have"
        };
    }

    this->m_spriteNumberOfCells =
        sf::Vector2u { static_cast< unsigned int >( numberOfRow ),
                       static_cast< unsigned int >( numberOfLine ) };
}

void Player::set_direction( E_Direction const & direction )
{
    this->m_direction = direction;
}

void Player::set_state( Player::E_State const & playerState )
{
    this->m_state = playerState;
}

void Player::update_extra( float deltaTime )
{
    this->update_delta_time( deltaTime );
    this->update_movement();
    this->update_texture_rect();

    this->m_lastState = { this->m_state, this->m_direction };
}

void Player::render( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    target.draw( this->m_sprite, states );
}

void Player::update_delta_time( float const & deltaTime )
{
    this->m_deltaTime = deltaTime;
}

void Player::update_movement()
{
    float speedVariation { 0.f };
    switch ( this->m_state )
    {
    case Player::E_State::Walking :
        speedVariation = 1.f;
        break;
    case Player::E_State::Running :
        speedVariation = 2.f;
        break;
    default :
        speedVariation = 0.f;
        break;
    }

    this->move( this->get_movement() * this->m_speed * speedVariation
                * this->m_deltaTime );
}

void Player::update_texture_rect()
{
    // We choose the proper texture rect of the texture that we had loaded
    this->m_sprite.setTextureRect( this->get_current_texture_rect() );
}

void Player::update_events()
{
    this->set_state( Player::E_State::Normal );

    if ( input::is_pressed( sf::Keyboard::Z ) )
    {
        this->set_state( Player::E_State::Walking );
        this->set_direction( E_Direction::Up );
    }
    if ( input::is_pressed( sf::Keyboard::S ) )
    {
        this->set_state( Player::E_State::Walking );
        this->set_direction( E_Direction::Down );
    }
    if ( input::is_pressed( sf::Keyboard::Q ) )
    {
        this->set_state( Player::E_State::Walking );
        this->set_direction( E_Direction::Left );
    }
    if ( input::is_pressed( sf::Keyboard::D ) )
    {
        this->set_state( Player::E_State::Walking );
        this->set_direction( E_Direction::Right );
    }

    if ( input::is_pressed( sf::Keyboard::Z ) )
    {
        this->set_state( Player::E_State::Running );
    }
}

sf::Vector2f Player::get_movement() const
{
    sf::Vector2f movementDistance { 0.f, 0.f };
    float const distance { 1.f };

    switch ( this->m_direction )
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
        ASSERTION( false, "Unknown Direction"s );
        break;
    }

    return movementDistance;
}

unsigned int Player::get_current_sprite_number()
{
    std::vector< unsigned int > const spriteNumbers {
        this->m_spriteValue.at( this->m_state ).at( this->m_direction )
    };

    float const timesPerFrame { 0.2f };
    this->m_timeElapsed += this->m_deltaTime;

    if ( this->m_lastState.state != this->m_state
         || this->m_lastState.direction != this->m_direction )
    {
        this->reset_last_sprite_index();
        this->m_timeElapsed = 0.f;
    }
    // To make an animation we must have a turnover of sprites
    else if ( this->m_timeElapsed > timesPerFrame )
    {
        if ( this->m_lastSpriteIndex == spriteNumbers.size() - 1 )
        {
            this->reset_last_sprite_index();
        }
        else
        {
            ++this->m_lastSpriteIndex;
        }

        this->m_timeElapsed = 0.f;
    }

    ASSERTION( ! spriteNumbers.empty(),
               "No information about what sprite to print"s );
    ASSERTION( this->m_lastSpriteIndex < spriteNumbers.size(),
               "Issues with sprites numbers"s );

    return spriteNumbers[this->m_lastSpriteIndex];
}

sf::IntRect Player::get_current_texture_rect()
{
    int const currentSpriteNumber { static_cast< int >(
        this->get_current_sprite_number() ) };

    // Cast to make things easier to read
    sf::Vector2i const spriteSize {
        static_cast< int >( this->m_spritePixelSize.x ),
        static_cast< int >( this->m_spritePixelSize.y ),
    };

    // Transform the sprite value into the sprite position
    sf::Vector2i spritePosition {};
    spritePosition.x = currentSpriteNumber % spriteSize.x;
    spritePosition.y =
        ( currentSpriteNumber - spritePosition.x ) / spriteSize.x;

    sf::IntRect textureRect {};
    // Transform the sprite position into the rectangle of the sprite in the texture
    textureRect.left = spritePosition.x * spriteSize.x;
    textureRect.top  = spritePosition.y * spriteSize.y;
    // Assign the size of a sprite cell
    textureRect.width  = spriteSize.x;
    textureRect.height = spriteSize.y;

    return textureRect;
}