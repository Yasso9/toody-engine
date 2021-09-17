#include "game_state.hpp"

GameState::GameState(
    std::map< TextureKey, sf::Texture > const & textures,
    std::map< FontKey, sf::Font > const & fonts,
    sf::Vector2u const & windowSize
)
    : State( textures, fonts, windowSize, StateName::Game )
{
    this->init_map();

    this->m_player.create( this->m_textures.at( TextureKey::Player ) );
}

void GameState::init_map()
{
    this->m_tilemap.set_texture( this->m_textures.at( TextureKey::Tileset ) );

    this->m_view.reset( sf::FloatRect( this->m_player.get_position(), sf::Vector2f( 400.f, 400.f ) ) );
}

void GameState::handle_keyboard_press( std::string const & input )
{
    if ( input == "MainMenu" )
    {
        this->m_stateName = StateName::MainMenu;
    }
}

void GameState::handle_global_input()
{
    if ( this->m_keyboard.at( "MoveUp" ).second )
    {
        this->m_player.walk( Direction::Up );
    }
    if ( this->m_keyboard.at( "MoveDown" ).second )
    {
        this->m_player.walk( Direction::Down );
    }
    if ( this->m_keyboard.at( "MoveLeft" ).second )
    {
        this->m_player.walk( Direction::Left );
    }
    if ( this->m_keyboard.at( "MoveRight" ).second )
    {
        this->m_player.walk( Direction::Right );
    }

    if ( this->m_keyboard.at( "Run" ).second )
    {
        this->m_player.run();
    }

    if ( !this->m_keyboard.at( "MoveUp" ).second && !this->m_keyboard.at( "MoveDown" ).second &&
    !this->m_keyboard.at( "MoveLeft" ).second && !this->m_keyboard.at( "MoveRight" ).second )
    {
        this->m_player.stay();
    }
}

void GameState::update_map()
{
    sf::Vector2f viewCenter { this->m_player.get_position() };

    if ( viewCenter.x < this->m_view.getSize().x / 2 )
    {
        viewCenter.x = this->m_view.getSize().x / 2;
    }
    else if ( viewCenter.x > static_cast<float>(this->m_windowSize.x) - this->m_view.getSize().x / 2 )
    {
        viewCenter.x = static_cast<float>(this->m_windowSize.x) - this->m_view.getSize().x / 2;
    }

    if ( viewCenter.y < this->m_view.getSize().y / 2 )
    {
        viewCenter.y = this->m_view.getSize().y / 2;
    }
    else if ( viewCenter.y > static_cast<float>(this->m_windowSize.x) - this->m_view.getSize().y / 2 )
    {
        viewCenter.y = static_cast<float>(this->m_windowSize.x) - this->m_view.getSize().y / 2;
    }

    this->m_view.setCenter( viewCenter );
    this->m_view.setSize( sf::Vector2f( 400.f, 400.f ) );
}


void GameState::update()
{
    this->update_map();
    this->handle_global_input();

    this->m_player.update();
}

void GameState::render( sf::RenderWindow & target )
{
    target.setView( this->m_view );

    target.draw( this->m_tilemap );

    this->m_player.render( target );
}