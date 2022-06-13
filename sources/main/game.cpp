#include "game.hpp"

#include <sstream>

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/exceptions.hpp"
#include "tools/string.hpp"

#include "main/settings.hpp"
#include "main/window.hpp"

#include "states/editor_state.hpp"
#include "states/game_state.hpp"
#include "states/graphic_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/test.hpp"

#include "libraries/imgui.hpp"

/**
 * @brief Throw an exception if something is not available
 */
static void check_configuration()
{
    if ( ! sf::Shader::isAvailable() )
    {
        throw std::runtime_error { "Shader's not available"s };
    }
}

Game::Game() : m_state( nullptr )
{
    check_configuration();

    Window::get_instance();
    if ( ! ImGui::SFML::Init( Window::get_instance() ) )
    {
        throw std::runtime_error { "Cannot init ImGui"s };
    }

    this->init_state();
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
    // Must be made before closing the window
    gl::check_error();
}

void Game::run()
{
    sf::Clock clock {};
    // Reset the clock juste before the game run
    clock.restart();

    bool gameShouldRun { true };

    double const refreshRate { Settings::get_instance().get_refresh_rate() };

    while ( gameShouldRun )
    {
        sf::Time const deltaTime { clock.getElapsedTime() };

        if ( deltaTime.asSeconds() > refreshRate )
        {
            /// @todo remove this try catch and put a more accurate thing to stop the game
            try
            {
                this->update_events();
                this->update_state( deltaTime );
                this->render();
            }
            catch ( Exception::QuitApplication const & )
            {
                gameShouldRun = false;
            }

            // reset the counter
            clock.restart();
        }
    }
}

void Game::init_state()
{
    // this->change_state( State::E_List::MainMenu );
    this->change_state( State::E_List::Editor );
    // this->change_state( State::E_List::Test );
    // this->change_state( State::E_List::Graphics );
}

void Game::update_events()
{
    sf::Event event {};
    // The event loop must always be part of the main loop,
    // otherwise bug and crash could happen
    while ( Window::get_instance().pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
        {
            throw Exception::QuitApplication {};
            return;
        }
        else if ( Window::get_instance().has_absolute_focus() )
        {
            ImGui::SFML::ProcessEvent( Window::get_instance(), event );
            this->m_state->update_inputs( event );
        }
    }

    if ( Window::get_instance().has_absolute_focus() )
    {
        this->m_state->extra_events();
    }

    ImGui::P_ResetWindowFocused();
}

void Game::update_state( sf::Time const & deltaTime )
{
    ImGui::SFML::Update( Window::get_instance(), deltaTime );

    static State::E_List lastState { this->m_state->get_state_to_print() };

    this->m_state->update_data( deltaTime.asSeconds() );

    State::E_List const newState { this->m_state->get_state_to_print() };
    if ( lastState != newState )
    {
        this->change_state( newState );
        lastState = newState;
    }
}

void Game::render()
{
    Window::get_instance().clear_all( sf::Color { 40, 40, 40 } );

    this->m_state->render();

    // We render after our state render, so the imGui's windows
    // will be drawn if we have a background
    ImGui::SFML::Render( Window::get_instance() );

    // Each draw is a chance that we have an error with OpenGL
    gl::check_error();

    Window::get_instance().display();
}

void Game::change_state( State::E_List const & newState )
{
    // Base configuration for cursor
    // Can be changed for a specific state
    Window::get_instance().setMouseCursorVisible( true );
    Window::get_instance().setMouseCursorGrabbed( false );

    switch ( newState )
    {
    case State::E_List::MainMenu :
        this->m_state = std::make_shared< MainMenuState >();
        break;
    case State::E_List::Game :
        this->m_state = std::make_shared< GameState >();
        break;
    case State::E_List::Editor :
        this->m_state = std::make_shared< EditorState >();
        break;
    case State::E_List::Graphics :
        // Window::get_instance().setMouseCursorVisible( false );
        // Window::get_instance().setMouseCursorGrabbed( true );
        this->m_state = std::make_shared< GraphicState >();
        break;
    case State::E_List::Test :
        this->m_state = std::make_shared< TestState >();
        break;

    case State::E_List::Quit :
        throw Exception::QuitApplication {};
        break;

    default :
        std::stringstream debugMessage {};
        debugMessage << "State::E_List "s << Enum< State::E_List > { newState }
                     << " unsupported"s;
        ASSERTION( false, debugMessage.str() );
        break;
    }
}