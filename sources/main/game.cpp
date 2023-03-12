#include "game.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl
#include <sstream>   // for stringstream
#include <string>    // for operator""s, operator<<

#include <IMGUI/imgui-SFML.h>       // for Init, ProcessEvent, Render
#include <SFML/Graphics/Color.hpp>  // for Color
#include <SFML/Window/Event.hpp>    // for Event, Event::Closed

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

    this->change_state( State::E_List::MainMenu );
}

Game::~Game()
{
    std::cout << "Game Ended" << std::endl;
}

void Game::update_all( UpdateContext context )
{
    ImGui::SFML::Update( sf::Mouse::getPosition( context.window ),
                         context.window.get_size().to_float(),
                         sf::seconds( context.deltaTime ) );

    this->Component::update_all( context );

    {  // Update the current state to show
        /// @todo use something like signal to change state
        static State::E_List lastState { m_state->get_state_to_print() };

        m_state->update_all( context );

        State::E_List const newState { m_state->get_state_to_print() };
        if ( lastState != newState )
        {
            lastState = newState;
            this->change_state( newState );
        }
    }
}

void Game::render_all( RenderContext & context ) const
{
    /// @todo utiliser render à la place de m_window
    context.window.clear();

    this->Component::render_all( context );
    m_state->render_all( context );

    // We render after our state render, so the imGui's windows
    // will be drawn if we have a background
    ImGui::SFML::Render( context.window );

    // Each draw is a chance that we have an error with OpenGL
    ASSERTION( gl::check_error(), "OpenGL error" );

    context.window.display();
}

void Game::update_inputs( Window & window )
{
    /// @todo Mettre ces reset autre part ou changer de method pour les reset
    // Reset mouse scroll
    input::set_mouse_scroll( 0.f );
    input::reset_mouse_movement();
    /// @todo c'est quoi ce truc !?
    m_state->clear_buttons();

    sf::Event event {};
    // The event loop must always be part of the main loop,
    // otherwise bug and crash could happen
    while ( window.pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
        {
            m_shouldRun = false;
        }
        else if ( window.has_absolute_focus() )
        {
            ImGui::SFML::ProcessEvent( window, event );
            /// @todo supprimer la fonction update_inputs. Doit etre dans le
            /// update
            m_state->update_inputs( event );
        }
    }
}

bool Game::should_run() const
{
    return m_shouldRun;
}

void Game::change_state( State::E_List const & newState )
{
    // Base configuration for cursor
    // Can be changed for a specific state
    // m_window.setMouseCursorVisible( true );
    // m_window.setMouseCursorGrabbed( false );

    switch ( newState )
    {
    case State::E_List::MainMenu :
        m_state = std::make_shared< MainMenuState >();
        break;
    case State::E_List::Editor :
        m_state = std::make_shared< EditorState >();
        break;
    case State::E_List::Graphics :
        // m_window.setMouseCursorVisible( false );
        // m_window.setMouseCursorGrabbed( true );
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