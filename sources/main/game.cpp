#include "game.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl
#include <sstream>   // for stringstream
#include <string>    // for operator""s, operator<<

#include <IMGUI/imgui-SFML.h>        // for Init, ProcessEvent, Render
#include <SFML/Graphics/Color.hpp>   // for Color
#include <SFML/Graphics/Shader.hpp>  // for Shader
#include <SFML/System/Clock.hpp>     // for Clock
#include <SFML/System/Time.hpp>      // for Time
#include <SFML/Window/Event.hpp>     // for Event, Event::Closed

#include "graphics3D/openGL.hpp"       // for check_error
#include "input/input.hpp"             // for reset_mouse_movement, set_mous...
#include "main/render.hpp"             // for Render
#include "main/settings.hpp"           // for Settings
#include "main/window.hpp"             // for Window
#include "states/editor_state.hpp"     // for EditorState
#include "states/graphic_state.hpp"    // for GraphicState
#include "states/main_menu_state.hpp"  // for MainMenuState
#include "states/test.hpp"             // for TestState
#include "tools/assertion.hpp"         // for ASSERTION
#include "tools/enumeration.hpp"       // for Enum
#include "tools/enumeration.tpp"       // for operator<<, Enum::Enum<Type>
#include "tools/exceptions.hpp"        // for System
#include "tools/singleton.tpp"         // for Singleton::get_instance

Game::Game() : m_state( nullptr ), m_shouldRun( true )
{
    std::cout << "Game Started" << std::endl;

    if ( ! ImGui::SFML::Init( Window::get_instance() ) )
    {
        throw exception::System { "Cannot init ImGui"s };
    }

    if ( ! sf::Shader::isAvailable() )
    {
        throw exception::System { "Shader's not available"s };
    }

    // Disable use of imgui.ini file
    // ImGui::GetIO().IniFilename = nullptr;

    this->init_state();
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
    // Must be made before closing the window
    gl::check_error();

    std::cout << "Game Ended" << std::endl;
}

void Game::run()
{
    sf::Clock clock {};
    // Reset the clock juste before the game run
    clock.restart();

    double const refreshRate { Settings::get_instance().get_refresh_rate() };

    while ( m_shouldRun )
    {
        sf::Time const deltaTime { clock.getElapsedTime() };

        if ( deltaTime.asSeconds() > refreshRate )
        {
            this->update_events();
            this->update_state( deltaTime );
            this->render();

            // reset the counter
            clock.restart();
        }
    }

    Window::get_instance().close();
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
    input::set_mouse_scroll( 0.f );
    input::reset_mouse_movement();

    sf::Event event {};
    m_state->clear_buttons();
    // The event loop must always be part of the main loop,
    // otherwise bug and crash could happen
    while ( Window::get_instance().pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
        {
            m_shouldRun = false;
        }
        else if ( Window::get_instance().has_absolute_focus() )
        {
            ImGui::SFML::ProcessEvent( Window::get_instance(), event );
            m_state->update_inputs( event );
        }
    }
}

void Game::update_state( sf::Time const & deltaTime )
{
    ImGui::SFML::Update( Window::get_instance(), deltaTime );

    /// @todo use something like signal to change state

    static State::E_List lastState { m_state->get_state_to_print() };

    m_state->update_all( deltaTime.asSeconds() );

    State::E_List const newState { m_state->get_state_to_print() };
    if ( lastState != newState )
    {
        lastState = newState;
        this->change_state( newState );
    }
}

void Game::render()
{
    Window::get_instance().clear_all( sf::Color { 0, 0, 0 } );

    Render render { Window::get_instance() };
    m_state->render_all( render );

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
        m_state = std::make_shared< MainMenuState >();
        break;
    case State::E_List::Editor :
        m_state = std::make_shared< EditorState >();
        break;
    case State::E_List::Graphics :
        // Window::get_instance().setMouseCursorVisible( false );
        // Window::get_instance().setMouseCursorGrabbed( true );
        m_state = std::make_shared< GraphicState >();
        break;
    case State::E_List::Test :
        m_state = std::make_shared< TestState >();
        break;

    case State::E_List::Quit :
        m_shouldRun = false;
        break;

    default :
        std::stringstream debugMessage {};
        debugMessage << "State::E_List "s << Enum< State::E_List > { newState }
                     << " unsupported"s;
        ASSERTION( false, debugMessage.str() );
        break;
    }
}