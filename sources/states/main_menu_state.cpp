#include "main_menu_state.hpp"

#include "main/resources.hpp"
#include "tools/string.hpp"

MainMenuState::MainMenuState() : State( State::E_List::MainMenu )
{
    this->init_background();
    this->init_text();
    this->init_buttons();
}

void MainMenuState::extra_events()
{
    int const buttonNumberPressed { this->m_buttons.update(
        // TYPO essayer d'enlever le static cast, ça fait moche
        static_cast< sf::Vector2f >(
            sf::Mouse::getPosition( Window::get_instance() ) ),
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_stateName = static_cast< State::E_List >( buttonNumberPressed );
    }
}

void MainMenuState::update() {}

void MainMenuState::render() const
{
    Window::get_instance().draw( this->m_background );

    Window::get_instance().draw( this->m_text );

    Window::get_instance().draw( this->m_buttons );
}

void MainMenuState::init_background()
{
    this->m_background.setFillColor( sf::Color( 82, 88, 92 ) );

    this->m_background.setSize( this->m_settings.get_window_size_f() );
    this->m_background.setPosition( 0.f, 0.f );
}

void MainMenuState::init_text()
{
    this->m_text.setFont(
        Resources::get_instance().get_font( Resources::E_FontKey::Arial ) );
    this->m_text.setCharacterSize( 30u );
    this->m_text.setFillColor( sf::Color::Black );
    this->m_text.setPosition( 100.f, 500.f );
    this->m_text.setString( "Main Menu"s );
}

void MainMenuState::init_buttons()
{
    this->m_buttons.set_font(
        Resources::get_instance().get_font( Resources::E_FontKey::Arial ) );
    this->m_buttons.set_direction( ButtonArray::E_Direction::Horizontal );
    // TYPO ne plus hardcodé ça mais trouvé un autre moyen avec la State::E_List
    this->m_buttons.set_strings(
        { "Main Menu"s, "Game"s, "Editor"s, "Graphics"s, "Test"s, "Exit"s } );
    this->m_buttons.setPosition( 0.f, 0.f );
}