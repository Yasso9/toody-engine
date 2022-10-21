#pragma once

#include <memory>  // for shared_ptr

#include "states/state.hpp"  // for State, State::E_List

namespace sf
{
    class Time;
}  // namespace sf

class Game final
{
  private:
    std::shared_ptr< State > m_state;
    bool                     m_shouldRun;

  public:
    Game();
    virtual ~Game();

    void run ();

  private:
    /// @brief To know where do we start the game
    void init_state ();

    /// @brief update input change
    void update_events ();
    /// @brief Update all the things that can happen in the state
    void update_state ( sf::Time const & deltaTime );

    /// @render the game
    void render ();

    /**
     * @brief change the state to a new state
     * @param newState the new state to be printed
     */
    void change_state ( State::E_List const & newState );
};