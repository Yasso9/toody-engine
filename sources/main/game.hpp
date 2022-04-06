#pragma once

#include <memory>

#include "settings/settings.hpp"
#include "states/state.hpp"
#include "tools/types.hpp"

class Game final
{
  public:
    Game();
    virtual ~Game() = default;

    void run();

  private:
    std::shared_ptr< State > m_states;

    Ressources const m_ressources;

    sf::Event m_event;
    sf::Clock m_clock;

    Settings const m_settings;

    void init_window();
    /// @brief To know where do we start the game
    void init_state();

    void update_events();
    /// @brief Update all the things that can happen in the state
    void update_state( float const & deltaTime );

    void render();

    /**
     * @brief change the state to a new state
     * @param newState the new state to be printed
     */
    void change_state( State::E_List const & newState );

    template < typename StateClass >
    void set_new_state()
    {
        this->m_states = std::make_shared< StateClass >( this->m_ressources );
    }
};