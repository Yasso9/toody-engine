#include "main_menu_state.hpp"

#include "tools/string.hpp"

MainMenuState::MainMenuState( std::shared_ptr< sf::RenderWindow > window,
                              Ressources const & ressources )
  : State( window, ressources, State::E_List::MainMenu )
{
    this->init_background();
    this->init_text();
    this->init_buttons();
}

void MainMenuState::update()
{
    int const buttonNumberPressed { this->m_buttons.update(
        // TYPO essayer d'enlever le static cast, ça fait moche
        static_cast< sf::Vector2f >(
            sf::Mouse::getPosition( *this->m_window ) ),
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_stateName = static_cast< State::E_List >( buttonNumberPressed );
    }
}

void MainMenuState::render()
{
    // Reset the view (if an other state had changed it)
    this->m_window->setView( this->m_window->getDefaultView() );

    this->m_window->draw( this->m_background );

    this->m_window->draw( this->m_text );

    this->m_window->draw( this->m_buttons );
}

void MainMenuState::init_background()
{
    this->m_background.setFillColor( sf::Color( 225, 155, 155 ) );

    this->m_background.setSize( this->m_settings.get_window_size_f() );
    this->m_background.setPosition( 0.f, 0.f );
}

void MainMenuState::init_text()
{
    this->m_text.setFont( this->m_ressources.fonts.at( E_FontKey::Arial ) );
    this->m_text.setCharacterSize( 30u );
    this->m_text.setFillColor( sf::Color::Black );
    this->m_text.setPosition( 100.f, 500.f );
    this->m_text.setString( "Main Menu"s );
}

void MainMenuState::init_buttons()
{
    this->m_buttons.set_font( this->m_ressources.fonts.at( E_FontKey::Arial ) );
    this->m_buttons.set_direction( ButtonArray::E_Direction::Horizontal );
    // TYPO ne plus hardcodé ça mais trouvé un autre moyen avec la State::E_List
    this->m_buttons.set_strings(
        { "Main Menu"s, "Game"s, "Editor"s, "Graphics", "Test", "Exit"s } );
    this->m_buttons.setPosition( 0.f, 0.f );
}