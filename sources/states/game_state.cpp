#include "game_state.hpp"

#include "main/window.hpp"      // for Window
#include "maths/vector2.hpp"    // for Vector2, Vector2U, Vector2F
#include "maths/vector2.tpp"    // for operator/, operator-, Vector2::operat...
#include "tools/singleton.tpp"  // for Singleton::get_instance

GameState::GameState()
  : State( State::E_List::Game ), m_view(), m_tilemap( m_view ), m_player()
{
    this->init_map();
}

void GameState::init_map()
{
    this->m_tilemap.setPosition(
        ( math::Vector2F { Window::get_instance().get_size() }
          - this->m_tilemap.get_size() )
        / 2.f );
    this->m_player.setPosition(
        ( math::Vector2F { Window::get_instance().get_size() }
          - this->m_tilemap.get_size() )
        / 2.f );

    this->m_view.setCenter( this->m_player.getPosition() );
    this->m_view.setSize( Window::get_instance().get_size().to_float() / 2.f );
}

void GameState::update_before( float deltaTime )
{
    this->m_player.update( deltaTime );
    // The view follow the player
    this->m_view.setCenter( this->m_player.getPosition() );
}

// void GameState::render( sf::RenderTarget & target ) const
// {
//     // Window::get_instance().setView( this->m_view );

// target.draw( this->m_tilemap );

// target.draw( this->m_player );

// // Window::get_instance().reset_view();}