#pragma once

#include <memory>

#include <project/settings/settings.hpp>
#include <project/states/state.hpp>
#include <project/tools/types.hpp>

class Game final
{
  public:
    Game();

    Game & operator=( const Game & ) = delete;
    Game & operator=( Game && ) = delete;
    Game( const Game & ) = delete;
    Game( Game && ) = delete;

    void run();

    void update_events();
    void update_state();

    void render();

    virtual ~Game() {}

  private:
    std::shared_ptr<State> m_states { nullptr };

    TexturesMap const m_textures {};
    FontsMap const m_fonts {};

    sf::RenderWindow m_window {};
    sf::Event m_event {};
    sf::Clock m_clock {};

    Settings const m_settings {};

    /// @brief state that was print before the update of the state
    StateName m_lastState {};

    void init_window();
    void init_state();

    /** @brief change the state to a new state
     * @param newState StateName value of the new state
     */
    void change_state( StateName const & newState );
};