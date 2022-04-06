#include "game.hpp"

#include <sstream>

#include "tools/assertion.hpp"
#include "tools/enumeration.hpp"
#include "tools/string.hpp"

#include "ressources_handling/ressources_init.hpp"

#include "main/window.hpp"

#include "states/editor_state.hpp"
#include "states/game_state.hpp"
#include "states/graphic_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/test.hpp"

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

Game::Game()
  : m_states( nullptr ),
    m_settings(),
    m_ressources( RessourcesInit::init_textures(),
                  RessourcesInit::init_fonts() )
{
    check_configuration();

    // Assurance that the window will be created here
    Window::get_instance();

    this->init_state();

    gl::initialize();
}

void Game::init_state()
{
    this->change_state( State::E_List::MainMenu );
    // this->change_state( State::E_List::Test );
    // this->change_state( State::E_List::Graphics );
}

void Game::run()
{
    float deltaTime { 0.f };

    // Reset the clock juste before the game run
    this->m_clock.restart();

    while ( Window::get_instance().isOpen() )
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
    while ( Window::get_instance().pollEvent( this->m_event ) )
    {
        if ( Window::get_instance().hasFocus() )
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
    Window::get_instance().clear( sf::Color::White );

    this->m_states->render();

    Window::get_instance().display();
}

void Game::change_state( State::E_List const & newState )
{
    switch ( newState )
    {
    case State::E_List::MainMenu :
        Window::get_instance().setMouseCursorVisible( true );
        Window::get_instance().setMouseCursorGrabbed( false );
        this->set_new_state< MainMenuState >();
        break;
    case State::E_List::Game :
        Window::get_instance().setMouseCursorVisible( true );
        Window::get_instance().setMouseCursorGrabbed( false );
        this->set_new_state< GameState >();
        break;
    case State::E_List::Editor :
        Window::get_instance().setMouseCursorVisible( true );
        Window::get_instance().setMouseCursorGrabbed( false );
        this->set_new_state< EditorState >();
        break;
    case State::E_List::Graphics :
        // TYPO n'activer le grab de la souris que si la window est focus
        // Window::get_instance().setMouseCursorVisible( false );
        // Window::get_instance().setMouseCursorGrabbed( true );
        Window::get_instance().setMouseCursorVisible( true );
        Window::get_instance().setMouseCursorGrabbed( false );
        this->set_new_state< GraphicState >();
        break;
    case State::E_List::Test :
        Window::get_instance().setMouseCursorVisible( true );
        Window::get_instance().setMouseCursorGrabbed( false );
        this->set_new_state< TestState >();
        break;

    case State::E_List::Quit :
        Window::get_instance().close();
        break;

    default :
        std::stringstream debugMessage {};
        debugMessage << "State::E_List "s << Enum< State::E_List > { newState }
                     << " unsupported"s;
        ASSERTION( false, debugMessage.str() );
        break;
    }
}
