#include "main_menu_state.hpp"

#include <project/tools/string.hpp>

MainMenuState::MainMenuState( Ressources const & ressources,
                              Settings const & settings )
  : State( ressources, settings, StateName::MainMenu ),
    m_buttons( this->m_ressources.fonts.at( FontKey::Arial ),
               { "Main Menu"s, "Game"s, "Editor"s, "Exit"s } )
{
    this->init_background();
    this->init_text();
}

void MainMenuState::init_background()
{
    this->m_background.setFillColor( sf::Color( 225, 155, 155 ) );

    this->m_background.setSize( this->m_settings.get_window_size_f() );
    this->m_background.setPosition( 0.f, 0.f );
}

void MainMenuState::init_text()
{
    this->m_text.setFont( this->m_ressources.fonts.at( FontKey::Arial ) );
    this->m_text.setCharacterSize( 30 );
    this->m_text.setFillColor( sf::Color::Black );
    this->m_text.setPosition( 100.f, 200.f );
    this->m_text.setString( "Main Menu"s );
}

void MainMenuState::update()
{
    // if ( this->m_mouseButton.at( "AcceptClick"s ).second )
    // {
    //     this->m_stateName =
    //         static_cast<StateName>( this->m_buttons.update_press(
    //             this->m_mousePosition.get_press(),
    //             static_cast<int>( this->m_stateName ) ) );
    // }

    int const buttonNumberPressed { this->m_buttons.update(
        this->m_mousePosition.get_overall(),
        this->m_mouseButton.at( "AcceptClick"s ).second ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_stateName = static_cast<StateName>( buttonNumberPressed );
    }
}

void MainMenuState::render( sf::RenderWindow & target )
{
    target.setView( target.getDefaultView() );

    target.draw( this->m_background );

    target.draw( this->m_text );

    target.draw( this->m_buttons );
}