#include "game.hpp"

Game::Game()
  : m_textures( RessourcesInit::init_textures() ),
    m_fonts( RessourcesInit::init_fonts() ),
    m_timePerFrame( 1. / 60. )
{
    this->init_window();
    this->init_state();
}

void Game::init_window()
{
    std::ifstream file { "./ressources/window.txt", std::ios::in };
    if ( ! file )
    {
        throw std::runtime_error { "There's no file named ./ressources/window.txt" };
    }

    unsigned int windowWidth, windowHeight;
    bool verticalSync;

    file >> windowWidth >> windowHeight >> verticalSync;

    this->m_window.create( sf::VideoMode( windowWidth, windowHeight ),
                           "Toody Engine (In Developpement)" );
    this->m_window.setVerticalSyncEnabled( verticalSync );
}

void Game::init_state()
{
    this->m_states = std::make_shared<MainMenuState>(
        this->m_textures, this->m_fonts, this->m_window.getSize() );

    this->m_lastState = this->m_states->get_next();
}

void Game::run()
{
    this->m_clock.restart();

    float deltaTime { 0.f };

    while ( this->m_window.isOpen() )
    {
        deltaTime += this->m_clock.restart().asSeconds();

        if ( deltaTime > this->m_timePerFrame )
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

    if ( this->m_states->get_next() != this->m_lastState )
    {
        this->change_state( this->m_states->get_next() );
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
                this->m_textures, this->m_fonts, this->m_window.getSize() );
        }
        break;

    case StateName::Game :
        {
            this->m_states = std::make_shared<GameState>(
                this->m_textures, this->m_fonts, this->m_window.getSize() );
        }
        break;

    case StateName::Editor :
        {
            this->m_states = std::make_shared<EditorState>(
                this->m_textures, this->m_fonts, this->m_window.getSize() );
        }
        break;

    case StateName::Quit : this->m_window.close(); break;

    default : throw std::invalid_argument( "StateName unsupported" ); break;
    }
}

void Game::render()
{
    this->m_window.clear( sf::Color::White );
    this->m_states->render( this->m_window );
    this->m_window.display();
}
