#include "game_state.hpp"

#include "main/resources.hpp"

// TYPO changer les arguments en une structure comprenant T_TexturesMap et T_FontsMap
GameState::GameState()
  : State( State::E_List::Game ),
    m_tilemap( Resources::get_instance().get_texture(
        Resources::E_TextureKey::Tileset ) ),
    m_player( Resources::get_instance().get_texture(
        Resources::E_TextureKey::Player ) )
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

void GameState::update_map()
{
    // The view follow the player
    this->m_view.setCenter( this->m_player.getPosition() );
}

void GameState::update()
{
    this->m_player.set_state( Player::E_State::Normal );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Up );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Down );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Left );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        this->m_player.set_state( Player::E_State::Walking );
        this->m_player.set_direction( E_Direction::Right );
    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->m_player.set_state( Player::E_State::Running );
    }

    this->m_player.update( this->m_deltaTime );
    this->update_map();
}

void GameState::render() const
{
    Window::get_instance().setView( this->m_view );

    Window::get_instance().draw( this->m_tilemap );

    Window::get_instance().draw( this->m_player );
}