#include "game.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl
#include <sstream>   // for stringstream
#include <string>    // for operator""s, operator<<

#include <IMGUI/imgui-SFML.h>        // for Init, ProcessEvent, Render
#include <SFML/Graphics/Color.hpp>   // for Color
#include <SFML/Graphics/Shader.hpp>  // for Shader
#include <SFML/Window/Event.hpp>     // for Event, Event::Closed

#include "graphics3D/openGL.hpp"  // for check_error
#include "input/input.hpp"        // for reset_mouse_movement, set_mous...
#include "main/render.hpp"        // for Render
#include "main/settings.hpp"      // for Settings
#include "main/window.hpp"        // for Window
#include "states/editor.hpp"      // for EditorState
#include "states/graphic.hpp"     // for GraphicState
#include "states/main_menu.hpp"   // for MainMenuState
#include "states/test.hpp"        // for TestState
#include "tools/assertion.hpp"    // for ASSERTION
#include "tools/enumeration.hpp"  // for Enum
#include "tools/enumeration.tpp"  // for operator<<, Enum::Enum<Type>
#include "tools/singleton.tpp"    // for Singleton::get_instance
#include "tools/traces.hpp"

Game::Game() : m_state { nullptr }, m_shouldRun { true }
{
    std::cout << "Game Started" << std::endl;

    if ( ! ImGui::SFML::Init( Window::get_instance() ) )
    {
        Trace::Error( "Cannot init ImGui" );
    }
    if ( ! sf::Shader::isAvailable() )
    {
        Trace::Error( "Shader's not available" );
    }

    // To disable the use of imgui.ini file
    // ImGui::GetIO().IniFilename = nullptr;

    this->change_state( State::E_List::MainMenu );
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
    // Must be made before closing the window
    ASSERTION( gl::check_error(), "OpenGL error" );

    std::cout << "Game Ended" << std::endl;
}

void Game::update( float deltaTime )
{
    /// @todo Mettre ces reset autre part ou changer de method pour les reset
    // Reset mouse scroll
    input::set_mouse_scroll( 0.f );
    input::reset_mouse_movement();

    sf::Event event {};
    /// @todo c'est quoi ce truc !?
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
            /// @todo supprimer la fonction update_inputs. Doit etre dans le
            /// update
            m_state->update_inputs( event );
        }
    }

    // ImGui update and render
    ImGui::SFML::Update( Window::get_instance(), sf::seconds( deltaTime ) );

    {  // Update the current state to show
        /// @todo use something like signal to change state
        static State::E_List lastState { m_state->get_state_to_print() };

        m_state->update_all( deltaTime );

        State::E_List const newState { m_state->get_state_to_print() };
        if ( lastState != newState )
        {
            lastState = newState;
            this->change_state( newState );
        }
    }
}

void Game::render( Render & render ) const
{
    /// @todo utiliser render à la place de Window::get_instance()
    Window::get_instance().clear_all( sf::Color { 0, 0, 0 } );

    m_state->render_all( render );

    // We render after our state render, so the imGui's windows
    // will be drawn if we have a background
    ImGui::SFML::Render( Window::get_instance() );

    // Each draw is a chance that we have an error with OpenGL
    ASSERTION( gl::check_error(), "OpenGL error" );

    Window::get_instance().display();
}

bool Game::should_run() const
{
    return m_shouldRun;
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
        debugMessage << "State::E_List " << Enum< State::E_List > { newState }
                     << " unsupported";
        Trace::Error( debugMessage.str() );
        break;
    }
}