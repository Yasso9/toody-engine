#include "main_menu_state.hpp"

#include <string> // for string, allocator

#include <SFML/Graphics/Color.hpp>        // for Color, Color::Black
#include <SFML/Graphics/RenderTarget.hpp> // for RenderTarget
#include <SFML/System/Vector2.hpp>        // for Vector2f
#include <SFML/Window/Mouse.hpp>          // for Mouse, Mouse::Button, Mous...

#include "input/input.hpp"     // for get_mouse_position, is_pre...
#include "main/render.hpp"     // for Render
#include "main/resources.hpp"  // for get_font
#include "main/window.hpp"     // for Window
#include "maths/vector2.hpp"   // for Vector2, Vector2F, Vector2I
#include "maths/vector2.tpp"   // for Vector2::operator Vector2<...
#include "tools/singleton.tpp" // for Singleton::get_instance

MainMenuState::MainMenuState()
  : State( State::E_List::MainMenu ), m_background(), m_text(), m_buttons()
{
    this->init_background();
    this->init_text();
    this->init_buttons();
}

void MainMenuState::update_before( float /* deltaTime */ )
{
    int const buttonNumberPressed { this->m_buttons.update_button_array(
        static_cast< sf::Vector2f >( input::get_mouse_position().to_float() ),
        input::is_pressed( sf::Mouse::Button::Left ) ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_stateName = static_cast< State::E_List >( buttonNumberPressed );
    }
}

void MainMenuState::render_before( Render & render ) const
{
    render.get_target().draw( m_background, render.get_state() );
    render.get_target().draw( m_text, render.get_state() );
    m_buttons.render_all( render );
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
    this->m_text.setString( "Main Menu" );
}

void MainMenuState::init_buttons()
{
    this->m_buttons.set_font( resources::get_font( "arial.ttf" ) );
    this->m_buttons.set_direction( ButtonArray::E_Direction::Horizontal );
    this->m_buttons.set_strings(
        { "Main Menu", "Game", "Editor", "Graphics", "Test", "Exit" } );
    this->m_buttons.setPosition( 0.f, 0.f );
}