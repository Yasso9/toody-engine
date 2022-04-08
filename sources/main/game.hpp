#pragma once

#include <memory>

#include "settings/settings.hpp"
#include "states/state.hpp"

class Game final
{
  public:
    Game();
    virtual ~Game() = default;

    void run();

  private:
    // TYPO utiliser une exception pour gérer cette variable
    /// @brief true if the game should continue to run, false otherwise
    bool m_shouldRun;

    std::shared_ptr< State > m_states;

    Settings const m_settings;

    sf::Event m_event;
    sf::Clock m_clock;

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

    void quit();

    // TYPO à voir si on la supprime car cette fonction ne esimplifie pas grand chose
    template < typename StateClass >
    void set_new_state()
    {
        this->m_states = std::make_shared< StateClass >();
    }
};