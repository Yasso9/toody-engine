#include "game_state.hpp"

// TYPO changer les arguments en une structure comprenant TexturesMap et FontsMap
GameState::GameState( Ressources const & ressources, Settings const & settings )
  : State( ressources, settings, StateName::Game ),
    m_tilemap( ressources.textures.at( TextureKey::Tileset ) ),
    m_player( ressources.textures.at( TextureKey::Player ) )
{
    this->init_map();
}

void GameState::init_map()
{
    this->m_tilemap.setPosition(
        ( this->m_settings.get_window_size_f() - this->m_tilemap.get_size() )
        / 2.f );
    this->m_player.setPosition(
        ( this->m_settings.get_window_size_f() - this->m_tilemap.get_size() )
        / 2.f );

    this->m_view.setCenter( this->m_player.getPosition() );
    this->m_view.setSize( this->m_settings.get_window_size_f() / 2.f );
}

void GameState::handle_keyboard_press( std::string const & input )
{
    if ( input == "MainMenu" )
    {
        this->m_stateName = StateName::MainMenu;
    }
}

void GameState::handle_current_input()
{
    if ( this->m_keyboard.at( "MoveUp" ).second )
    {
        this->m_player.set_direction( E_Direction::Up );
    }
    if ( this->m_keyboard.at( "MoveDown" ).second )
    {
        this->m_player.set_direction( E_Direction::Down );
    }
    if ( this->m_keyboard.at( "MoveLeft" ).second )
    {
        this->m_player.set_direction( E_Direction::Left );
    }
    if ( this->m_keyboard.at( "MoveRight" ).second )
    {
        this->m_player.set_direction( E_Direction::Right );
    }

    if ( this->m_keyboard.at( "Run" ).second )
    {
        this->m_player.set_state( Player::E_State::Running );
    }
    else
    {
        this->m_player.set_state( Player::E_State::Walking );
    }

    // if ( ! this->m_keyboard.at( "MoveUp" ).second
    //      && ! this->m_keyboard.at( "MoveDown" ).second
    //      && ! this->m_keyboard.at( "MoveLeft" ).second
    //      && ! this->m_keyboard.at( "MoveRight" ).second )
    // {
    //     this->m_player.stay();
    // }
}

void GameState::update_map()
{
    // The view follow the player
    this->m_view.setCenter( this->m_player.getPosition() );
}

void GameState::update()
{
    this->m_player.update();
    this->update_map();
}

void GameState::render( sf::RenderWindow & target )
{
    target.setView( this->m_view );

    target.draw( this->m_tilemap );

    target.draw( this->m_player );
}