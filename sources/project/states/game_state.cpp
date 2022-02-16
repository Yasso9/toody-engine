#include "game_state.hpp"

// TYPO changer les arguments en une structure comprenant T_TexturesMap et T_FontsMap
GameState::GameState( std::shared_ptr<sf::RenderWindow> window,
                      Ressources const & ressources, Settings const & settings )
  : State( window, ressources, settings, State::E_List::Game ),
    m_tilemap( ressources.textures.at( E_TextureKey::Tileset ) ),
    m_player( ressources.textures.at( E_TextureKey::Player ) )
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
        this->m_stateName = State::E_List::MainMenu;
    }
}

void GameState::handle_current_input()
{
    this->m_player.set_state( Player::E_State::Normal );

    if ( this->m_keyboard.at( "MoveUp" ).second )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Up );
    }
    if ( this->m_keyboard.at( "MoveDown" ).second )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Down );
    }
    if ( this->m_keyboard.at( "MoveLeft" ).second )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Left );
    }
    if ( this->m_keyboard.at( "MoveRight" ).second )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Right );
    }

    if ( this->m_keyboard.at( "Run" ).second )
    {
        this->m_player.set_state( Player::E_State::Running );
    }
}

void GameState::update_map()
{
    // The view follow the player
    this->m_view.setCenter( this->m_player.getPosition() );
}

void GameState::update( float const & deltaTime )
{
    this->m_player.update( deltaTime );
    this->update_map();
}

void GameState::render()
{
    m_window->setView( this->m_view );

    m_window->draw( this->m_tilemap );

    m_window->draw( this->m_player );
}