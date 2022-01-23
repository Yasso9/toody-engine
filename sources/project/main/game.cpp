#include "game.hpp"

#include <cassert>

#include <project/ressources-handling/ressources_init.hpp>
#include <project/tools/string.hpp>

#include <project/states/editor_state.hpp>
#include <project/states/game_state.hpp>
#include <project/states/main_menu_state.hpp>

Game::Game()
  : m_textures( RessourcesInit::init_textures() ),
    m_fonts( RessourcesInit::init_fonts() ),
    m_settings()
{
    this->init_window();
    this->init_state();
}

void Game::init_window()
{
    std::string const gameTitle { "Toody Engine (In Developpement)"s };

    this->m_window.create(
        sfpp::to_video_mode( this->m_settings.get_window_size_u() ),
        gameTitle );
    this->m_window.setVerticalSyncEnabled(
        this->m_settings.get_vertical_sync() );
}

void Game::init_state()
{
    // Game start with main menu
    this->m_states = std::make_shared<MainMenuState>(
        Ressources { this->m_textures, this->m_fonts },
        this->m_settings );

    // The current state to print is the main menu (we've just set it)
    // We get this state because m_lastState should always know what is
    // the last state printed
    this->m_lastState = this->m_states->get_state_to_print();
}

void Game::run()
{
    this->m_clock.restart();

    float deltaTime { 0.f };

    while ( this->m_window.isOpen() )
    {
        deltaTime += this->m_clock.restart().asSeconds();

        if ( deltaTime > this->m_settings.get_refresh_rate() )
        {
            this->update_events();
            this->update_state();
            this->render();

            deltaTime = 0.f; // reset the counter
        }
    }
}

void Game::update_events()
{
    while ( this->m_window.pollEvent( this->m_event ) )
    {
        if ( this->m_window.hasFocus() )
        {
            this->m_states->update_input( this->m_event );
        }
    }
}

void Game::update_state()
{
    this->m_states->update();

    if ( this->m_states->get_state_to_print() != this->m_lastState )
    {
        this->change_state( this->m_states->get_state_to_print() );
    }
}

void Game::change_state( StateName const & newState )
{
    // this->m_states.pop();

    this->m_lastState = newState;

    switch ( newState )
    {
    case StateName::MainMenu :
        {
            this->m_states = std::make_shared<MainMenuState>(
                Ressources { this->m_textures, this->m_fonts },
                this->m_settings );
        }
        break;

    case StateName::Game :
        {
            this->m_states = std::make_shared<GameState>(
                Ressources { this->m_textures, this->m_fonts },
                this->m_settings );
        }
        break;

    case StateName::Editor :
        {
            this->m_states = std::make_shared<EditorState>(
                Ressources { this->m_textures, this->m_fonts },
                this->m_settings );
        }
        break;

    case StateName::Quit :
        this->m_window.close();
        break;

    default :
        assert( ( "StateName"s + std::to_string( static_cast<int>( newState ) )
                  + "unsupported"s )
                    .c_str() );
        break;
    }
}

void Game::render()
{
    this->m_window.clear( sf::Color::White );
    this->m_states->render( this->m_window );
    this->m_window.display();
}
