#include "game.hpp"

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/string.hpp"

#include "ressources_handling/ressources_init.hpp"

#include "states/editor_state.hpp"
#include "states/game_state.hpp"
#include "states/graphic_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/test.hpp"

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

    // TYPO à mettre autre part
    this->m_window.get()->setVisible( true );
    this->m_window.get()->requestFocus();
    this->m_window.get()->setKeyRepeatEnabled( false );
}

void Game::init_window()
{
    // We must use a pointer to copy this RenderWindow into another class
    // (RenderWindow doesn't have a copy contructor)
    this->m_window = std::make_shared<sf::RenderWindow>();

    // Additionnal settings that the window should have
    std::string const gameTitle { "Toody Engine (In Developpement)"s };
    sf::ContextSettings contextSettings {};
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.stencilBits = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 6;

    this->m_window->create(
        sfpp::to_video_mode( this->m_settings.get_window_size_u() ),
        gameTitle,
        sf::Style::Default,
        contextSettings );

    this->m_window->setVerticalSyncEnabled(
        this->m_settings.get_vertical_sync() );

    if ( ! this->m_window->setActive( true ) )
    {
        throw std::runtime_error {
            "Cannot set the windows as active state for OpenGL"s
        };
    }

    if ( ! sf::Shader::isAvailable() )
    {
        throw std::runtime_error { "Shader's not available"s };
    }
}

void Game::init_state()
{
    // this->change_state( State::E_List::MainMenu );
    // this->change_state( State::E_List::Test );
    this->change_state( State::E_List::Graphics );
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
            this->m_states->update_inputs( this->m_event );
        }
    }
}

void Game::update_state( float const & deltaTime )
{
    static State::E_List lastState { this->m_states->get_state_to_print() };

    this->m_states->update_data( deltaTime );

    State::E_List const newState { this->m_states->get_state_to_print() };
    if ( lastState != newState )
    {
        this->change_state( newState );
        lastState = newState;
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
    switch ( newState )
    {
    case State::E_List::MainMenu :
        this->m_window.get()->setMouseCursorGrabbed( false );
        this->set_new_state<MainMenuState>();
        break;
    case State::E_List::Game :
        this->m_window.get()->setMouseCursorGrabbed( false );
        this->set_new_state<GameState>();
        break;
    case State::E_List::Editor :
        this->m_window.get()->setMouseCursorGrabbed( false );
        this->set_new_state<EditorState>();
        break;
    case State::E_List::Graphics :
        this->m_window.get()->setMouseCursorGrabbed( true );
        this->set_new_state<GraphicState>();
        break;
    case State::E_List::Test :
        this->m_window.get()->setMouseCursorGrabbed( false );
        this->set_new_state<TestState>();
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
