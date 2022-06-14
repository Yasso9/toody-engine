#include "game_state.hpp"

#include "main/resources.hpp"

GameState::GameState()
  : State( State::E_List::Game ), m_view(), m_tilemap( m_view ), m_player()
{
    this->init_map();
}

void GameState::init_map()
{
    this->m_tilemap.setPosition(
        ( math::Vector2F { Window::get_instance().get_size_f() }
          - this->m_tilemap.get_size() )
        / 2.f );
    this->m_player.setPosition(
        ( math::Vector2F { Window::get_instance().get_size_f() }
          - this->m_tilemap.get_size() )
        / 2.f );

    this->m_view.setCenter( this->m_player.getPosition() );
    this->m_view.setSize( Window::get_instance().get_size_f() / 2.f );
}

void GameState::update()
{
    this->m_player.update( this->m_deltaTime );
    // The view follow the player
    this->m_view.setCenter( this->m_player.getPosition() );
}

void GameState::render() const
{
    Window::get_instance().setView( this->m_view );

    Window::get_instance().draw( this->m_tilemap );

    Window::get_instance().draw( this->m_player );

    Window::get_instance().reset_view();
}