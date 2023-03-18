#include "main_menu.hpp"

#include <sstream>
#include <string>  // for string, allocator

#include <SFML/Graphics/Color.hpp>         // for Color, Color::Black
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/System/Vector2.hpp>         // for Vector2f
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Button, Mous...

#include "input/input.hpp"  // for get_mouse_position, is_pre...
#include "libraries/imgui.hpp"
#include "main/game.hpp"
#include "main/resources.hpp"  // for get_font
#include "main/settings.hpp"
#include "main/window.hpp"  // for Window
#include "maths/geometry/rectangle.hpp"
#include "maths/vector2.hpp"  // for Vector2, Vector2F, Vector2I
#include "maths/vector2.tpp"  // for Vector2::operator Vector2<...
#include "tools/path.hpp"
#include "tools/singleton.tpp"  // for Singleton::get_instance

static math::RectangleF get_rectangle ( sf::Text text )
{
    constexpr float PADDING { 10.f };
    sf::FloatRect   localBounds { text.getLocalBounds() };
    math::Vector2F  textPosition { text.getPosition() };

    return math::RectangleF {
        textPosition.x + localBounds.left - PADDING,
        textPosition.y + localBounds.top - PADDING,
        localBounds.width + ( 2 * PADDING ),
        localBounds.height + ( 2 * PADDING ),
    };
}

static void reset_text_color ( std::vector< sf::Text > & texts )
{
    for ( sf::Text & text : texts )
    {
        text.setFillColor( sf::Color { 218, 218, 218 } );
    }
}

MainMenuState::MainMenuState( GameContext & gameContext )
  : State { gameContext }, m_texts {}, m_background {}, m_menuBackground {}
{
    math::Vector2F windowSize { Settings::get_instance().get_window_size() };

    m_background.setTexture( &resources::get_texture(
        path::get_folder( path::Resources ) / "images/main_menu.jpg" ) );
    m_background.setPosition( 0.f, 0.f );
    m_background.setSize( windowSize );

    m_menuBackground.setFillColor( sf::Color { 40, 40, 40, 180 } );
    m_menuBackground.setPosition( 50.f, 0.f );
    m_menuBackground.setSize( { windowSize.x / 5, windowSize.y } );

    for ( std::string stateName : State::get_state_list() )
    {
        m_texts.push_back(
            sf::Text { stateName,
                       resources::get_font( path::get_folder( path::Fonts )
                                            / "arial.ttf" ),
                       27u } );
    }

    math::Vector2F position { 150.f, 250.f };
    for ( sf::Text & text : m_texts )
    {
        text.setPosition( position );
        position.y += 60.f;
    }

    reset_text_color( m_texts );
}

void MainMenuState::update( UpdateContext context )
{
    // ImGui::P_Show( "Main Menu Debug", [&] () {
    //     std::stringstream output {};
    //     output << "Mouse Position : " << input::get_mouse_position_point()
    //            << "\n";
    //     ImGui::Text( "%s", output.str().c_str() );
    // } );
    static bool buttonHasBeenPressed { false };

    reset_text_color( m_texts );

    for ( sf::Text & text : m_texts )
    {
        if ( input::get_mouse_position( context.window )
                 .to_point()
                 .to_float()
                 .is_inside( get_rectangle( text ) ) )
        {
            // Color on hover
            text.setFillColor( sf::Color { 227, 139, 89 } );

            // There is a press on the button
            if ( m_inputs.is_pressed( sf::Mouse::Button::Left )
                 || ( buttonHasBeenPressed
                      && input::is_pressed( context.window,
                                            sf::Mouse::Button::Left ) ) )
            {
                buttonHasBeenPressed = true;
                // Color on pressed
                text.setFillColor( sf::Color { 217, 68, 35 } );
            }

            if ( m_inputs.is_released( sf::Mouse::Button::Left ) )
            {
                // Color when choosing
                text.setFillColor( sf::Color { 242, 255, 54 } );
                m_gameContext.transition_to(
                    State::get_enum_state( text.getString() ) );
            }
        }
    }
}

void MainMenuState::render( RenderContext context ) const
{
    context.draw( m_background );
    context.draw( m_menuBackground );

    for ( sf::Text const & text : m_texts )
    {
        context.draw( text );
    }
}