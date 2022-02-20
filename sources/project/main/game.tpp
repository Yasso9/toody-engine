#pragma once

#include "game.hpp"

template <typename StateClass>
void Game::set_new_state()
{
    this->m_states = std::make_shared<StateClass>( this->m_window,
                                                   this->m_ressources,
                                                   this->m_settings );
    this->m_states->initialize();
}