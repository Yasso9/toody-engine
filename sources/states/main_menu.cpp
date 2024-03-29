#include "main_menu.hpp"

#include <sstream>
#include <string>                          // for string, allocator

#include <SFML/Graphics/Color.hpp>         // for Color, Color::Black
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/System/Vector2.hpp>         // for Vector2f
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Button, Mous...

#include "application/contexts/game_context.hpp"
#include "application/resources.hpp"    // for get_font
#include "application/configuration.hpp"
#include "application/interface/window.hpp"  // for Window
#include "imgui/imgui.hpp"
#include "maths/geometry/rect.hpp"
#include "maths/vector2.hpp"    // for Vector2, Vector2F, Vector2I
#include "maths/vector2.tpp"    // for Vector2::operator Vector2<...
                                // for get_mouse_position, is_pre...
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

MainMenuState::MainMenuState()
  : State {}, m_texts {}, m_background {}, m_menuBackground {}
{
    math::Vector2F windowSize { Config::get_instance().get_window_size() };

    m_background.setTexture( &resource::image::get( "main_menu.jpg" ) );
    m_background.setPosition( 0.f, 0.f );
    m_background.setSize( windowSize );

    m_menuBackground.setFillColor( sf::Color { 40, 40, 40, 180 } );
    m_menuBackground.setPosition( 50.f, 0.f );
    m_menuBackground.setSize( { windowSize.x / 5, windowSize.y } );

    for ( std::string stateName : get_list< StateList >() )
    {
        m_texts.push_back( sf::Text {
            stateName, resource::font::get( "FiraMono-Regular.ttf" ), 27u } );
    }

    math::Vector2F position { 150.f, 250.f };
    for ( sf::Text & text : m_texts )
    {
        text.setPosition( position );
        position.y += 60.f;
    }

    reset_text_color( m_texts );
}

void MainMenuState::update( UpdateContext & context )
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
        if ( context.inputs.get_mouse_position()
                 .to_point()
                 .to_float()
                 .is_inside( get_rectangle( text ) ) )
        {
            // Color on hover
            text.setFillColor( sf::Color { 227, 139, 89 } );

            // There is a press on the button
            if (
                context.inputs.is_pressed( sf::Mouse::Button::Left )
                || ( buttonHasBeenPressed
                     && context.inputs.is_pressed( sf::Mouse::Button::Left ) ) )
            {
                buttonHasBeenPressed = true;
                // Color on pressed
                text.setFillColor( sf::Color { 217, 68, 35 } );
            }

            if ( context.inputs.is_released( sf::Mouse::Button::Left ) )
            {
                // Color when choosing
                text.setFillColor( sf::Color { 242, 255, 54 } );
                context.transition_to(
                    to_enum< StateList >( text.getString() ) );
            }
        }
    }
}

void MainMenuState::render( RenderContext & context ) const
{
    context.draw( m_background );
    context.draw( m_menuBackground );

    for ( sf::Text const & text : m_texts )
    {
        context.draw( text );
    }
}
