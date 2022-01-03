#pragma once

#include <project/ressources-handling/ressources_init.hpp>

#include <project/states/editor_state.hpp>
#include <project/states/game_state.hpp>
#include <project/states/main_menu_state.hpp>

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

    t_texturesMap const m_textures {};
    t_fontsMap const m_fonts {};

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