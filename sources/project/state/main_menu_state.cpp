#include "main_menu_state.hpp"

MainMenuState::MainMenuState(
    std::map< TextureKey, sf::Texture > const & textures,
    std::map< FontKey, sf::Font > const & fonts,
    sf::Vector2u const & windowSize
)
    : State( textures, fonts, windowSize, StateName::MainMenu )
{
    this->init_background();
    this->init_text();
    this->init_buttons();
}

void MainMenuState::init_background()
{
    this->m_background.setFillColor( sf::Color( 225, 155, 155 ) );

    this->m_background.setSize( static_cast<sf::Vector2f>(this->m_windowSize) );
    this->m_background.setPosition( 0.f, 0.f );
}

void MainMenuState::init_text()
{
    this->m_text.setFont( this->m_fonts.at( FontKey::Arial ) );
    this->m_text.setCharacterSize( 40 );
    this->m_text.setFillColor( sf::Color::Black );
    this->m_text.setPosition( 500.f, 500.f );
    this->m_text.setString( "Main Menu" );
}

void MainMenuState::init_buttons()
{
    this->m_buttons = ButtonVector {
        this->m_fonts.at( FontKey::Arial ),
        { "Main Menu", "Game", "Editor", "Exit" }
    };
}

void MainMenuState::update()
{
    if ( this->m_mouseButton.at( "AcceptClick" ).second )
    {
        this->m_stateName = static_cast<StateName>(
            this->m_buttons.update_press(
                this->m_mousePosition.get_press(),
                static_cast<int>(this->m_stateName)
            )
        );
    }

    this->m_buttons.update_select( this->m_mousePosition.get_overall() );
}

void MainMenuState::render( sf::RenderWindow & target )
{
    target.setView( target.getDefaultView() );

    target.draw( this->m_background );

    target.draw( this->m_text );

    this->m_buttons.render( target );
}