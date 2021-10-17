#pragma once

#include <project/ressources-handling/ressources_init.hpp>
#include <project/state/editor_state.hpp>
#include <project/state/game_state.hpp>
#include <project/state/main_menu_state.hpp>

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

    std::map<TextureKey, sf::Texture> const m_textures;
    std::map<FontKey, sf::Font> const m_fonts;

    sf::RenderWindow m_window {};
    sf::Event m_event {};
    sf::Clock m_clock {};

    // TYPO lire cette variable à l'aide du config.ini
    double const m_timePerFrame {};

    /// @brief state that was print before the update of the state
    StateName m_lastState {};

    void init_window();
    void init_state();

    /** @brief change the state to a new state
     * @param newState StateName value of the new state
     */
    void change_state( StateName const & newState );
};