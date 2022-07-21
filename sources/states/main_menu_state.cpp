#include "main_menu_state.hpp"

#include <iostream>

#include "input/input.hpp"
#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

MainMenuState::MainMenuState()
  : State( State::E_List::MainMenu ), m_background(), m_text(), m_buttons()
{
    this->init_background();
    this->init_text();
    this->init_buttons();
}

void MainMenuState::update_extra( float /* deltaTime */ )
{
    int const buttonNumberPressed { this->m_buttons.update_button_array(
        static_cast< sf::Vector2f >( input::get_mouse_position().to_float() ),
        input::is_pressed( sf::Mouse::Button::Left ) ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_stateName = static_cast< State::E_List >( buttonNumberPressed );
    }
}

void MainMenuState::render( Render & render ) const
{
    render.draw( this->m_background );

    render.draw( this->m_text );

    render.draw( this->m_buttons );
}

void MainMenuState::init_background()
{
    this->m_background.setFillColor( sf::Color( 82, 88, 92 ) );

    this->m_background.setSize(
        static_cast< math::Vector2F >( Window::get_instance().get_size() ) );
    this->m_background.setPosition( 0.f, 0.f );
}

void MainMenuState::init_text()
{
    this->m_text.setFont( resources::get_font( "arial.ttf" ) );
    this->m_text.setCharacterSize( 30u );
    this->m_text.setFillColor( sf::Color::Black );
    this->m_text.setPosition( 100.f, 500.f );
    this->m_text.setString( "Main Menu"s );
}

void MainMenuState::init_buttons()
{
    this->m_buttons.set_font( resources::get_font( "arial.ttf" ) );
    this->m_buttons.set_direction( ButtonArray::E_Direction::Horizontal );
    this->m_buttons.set_strings(
        { "Main Menu"s, "Game"s, "Editor"s, "Graphics"s, "Test"s, "Exit"s } );
    this->m_buttons.setPosition( 0.f, 0.f );
}