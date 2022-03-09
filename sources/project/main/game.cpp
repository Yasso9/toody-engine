#include "game.hpp"

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/string.hpp"

#include "ressources_handling/ressources_init.hpp"

#include "states/editor_state.hpp"
#include "states/game_state.hpp"
#include "states/graphic_state.hpp"
#include "states/main_menu_state.hpp"

#include <sstream>

Game::Game()
  : m_window( nullptr ),
    m_states( nullptr ),
    m_ressources( RessourcesInit::init_textures(),
                  RessourcesInit::init_fonts() ),
    m_settings()
{
    this->init_window();
    this->init_state();
}

void Game::init_window()
{
    // We must use a pointer to copy this RenderWindow into another class
    // (RenderWindow doesn't have a copy contructor)
    this->m_window = std::make_shared<sf::RenderWindow>();

    std::string const gameTitle { "Toody Engine (In Developpement)"s };

    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings {};
    contextSettings.depthBits = 24;
    // contextSettings.sRgbCapable = true;

    this->m_window->create(
        sfpp::to_video_mode( this->m_settings.get_window_size_u() ),
        gameTitle,
        sf::Style::Default,
        contextSettings );

    this->m_window->setVerticalSyncEnabled(
        this->m_settings.get_vertical_sync() );
}

void Game::init_state()
{
    // Game start with main menu
    this->set_new_state<MainMenuState>();
    // this->set_new_state<GraphicState>();

    // The current state to print is the main menu (we've just set it)
    // We get this state because m_lastState should always know what is
    // the last state printed
    this->m_lastState = this->m_states->get_state_to_print();
}

void Game::run()
{
    float deltaTime { 0.f };

    // Reset the clock juste before the game run
    this->m_clock.restart();

    while ( this->m_window->isOpen() )
    {
        sf::Time const timeElaspsed { this->m_clock.restart() };
        deltaTime += timeElaspsed.asSeconds();

        if ( deltaTime > this->m_settings.get_refresh_rate() )
        {
            this->update_events();
            this->update_state( deltaTime );
            this->render();

            deltaTime = 0.f; // reset the counter
        }
    }
}

void Game::update_events()
{
    while ( this->m_window->pollEvent( this->m_event ) )
    {
        if ( this->m_window->hasFocus() )
        {
            this->m_states->update_input( this->m_event );
        }
    }
}

void Game::update_state( float const & deltaTime )
{
    this->m_states->update( deltaTime );

    // TYPO essayer de trouver une meilleure technique pour changer d'état
    if ( this->m_states->get_state_to_print() != this->m_lastState )
    {
        this->change_state( this->m_states->get_state_to_print() );
    }
}

void Game::render()
{
    this->m_window->clear( sf::Color::White );

    this->m_states->render();

    this->m_window->display();
}

void Game::change_state( State::E_List const & newState )
{
    ASSERTION( this->m_lastState != newState,
               "When there is a change of state, the state must be new"s );

    this->m_lastState = newState;

    switch ( newState )
    {
    case State::E_List::MainMenu :
        this->set_new_state<MainMenuState>();
        break;
    case State::E_List::Game :
        this->set_new_state<GameState>();
        break;
    case State::E_List::Editor :
        this->set_new_state<EditorState>();
        break;
    case State::E_List::Graphics :
        this->set_new_state<GraphicState>();
        break;

    case State::E_List::Quit :
        this->m_window->close();
        break;

    default :
        std::stringstream debugMessage {};
        debugMessage << "State::E_List "s << Enum<State::E_List> { newState }
                     << " unsupported"s;
        ASSERTION( false, debugMessage.str() );
        break;
    }
}
