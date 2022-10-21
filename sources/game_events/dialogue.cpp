#include "dialogue.hpp"

#include <IMGUI/imgui.h>                   // for Begin, End, InputFloat
#include <SFML/Graphics/Color.hpp>         // for Color, Color::Black, Color...
#include <SFML/Graphics/Rect.hpp>          // for FloatRect
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/System/Vector2.hpp>         // for Vector2f, Vector2
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Right

#include "graphics2D/view.hpp"
#include "input/input.hpp"           // for get_mouse_position, is_pre...
#include "libraries/imgui.hpp"       // for P_ColorEditor, P_InputNumber
#include "libraries/imgui.hpp"       // for P_Begin
#include "main/render.hpp"           // for Render
#include "main/resources.hpp"        // for get_font
#include "main/window.hpp"           // for Window
#include "maths/geometry/point.hpp"  // for PointI, Point, PointF
#include "maths/geometry/point.tpp"  // for Point::Point<Type>, Point:...
#include "maths/vector2.tpp"         // for Vector2::to_float, Vector2...
#include "tools/exceptions.hpp"      // for System
#include "tools/singleton.tpp"       // for Singleton::get_instance

Dialogue::Dialogue()
  : m_shape {},
    m_text {},
    m_isEnabled { true },
    m_showCustomisation { true },
    m_textRemaining { "" },
    m_regularCursor {},
    m_moveCursor {}
{
    math::Vector2F const windowSizeF {
        Window::get_instance().get_size().to_float() };

    m_shape.setSize(
        { windowSizeF.x * ( 4.f / 5.f ), windowSizeF.y * ( 1.f / 5.f ) } );
    m_shape.setPosition( sf::Vector2f {
        ( windowSizeF.x - m_shape.getSize().x ) / 2.f,
        ( windowSizeF.y - m_shape.getSize().y ) * 0.8f,
    } );

    m_shape.setFillColor( sf::Color::White );
    m_shape.setOutlineThickness( 3.f );
    m_shape.setOutlineColor( sf::Color::Black );

    m_text.setFont( resources::get_font( "arial.ttf" ) );
    m_text.setCharacterSize( 30u );
    m_text.setFillColor( sf::Color::Red );

    this->set_current_text( m_textRemaining );

    if ( ! m_regularCursor.loadFromSystem( sf::Cursor::Arrow ) )
    {
        throw exception::System { "Cannot load regular Cursor" };
    }
    if ( ! m_moveCursor.loadFromSystem( sf::Cursor::SizeAll ) )
    {
        throw exception::System { "Cannot load move Cursor" };
    }
}

bool Dialogue::next()
{
    if ( ! this->is_enabled() )
    {
        return false;
    }

    std::string currentText { "" };

    if ( m_textRemaining.size() > CHARACTER_LIMIT )
    {
        currentText     = m_textRemaining.substr( 0, CHARACTER_LIMIT );
        m_textRemaining = m_textRemaining.substr(
            CHARACTER_LIMIT + 1, m_textRemaining.size() );
    }
    else
    {
        currentText = m_textRemaining;
    }

    this->set_current_text( currentText );

    return ! currentText.empty();
}

void Dialogue::add_text( std::string textToAdd )
{
    m_textRemaining += textToAdd;
}

void Dialogue::process_mouse_movement_customisation(
    math::Vector2I mouseMovement )
{
    if ( ! this->is_enabled() )
    {
        return;
    }

    static bool mouseIsInsideBox = false;

    // If the mouse was already inside and we have clicked in the right button
    if ( mouseIsInsideBox && input::is_pressed( sf::Mouse::Right ) )
    {
        this->m_shape.move( mouseMovement.to_float() );
    }

    math::PointI mousePosition { input::get_mouse_position() };
    mouseIsInsideBox = mousePosition.to_float().is_inside(
        math::PointF { this->m_shape.getPosition() }, this->m_shape.getSize() );

    if ( mouseIsInsideBox )
    {
        Window::get_instance().setMouseCursor( m_moveCursor );
    }
    else
    {
        Window::get_instance().setMouseCursor( m_regularCursor );
    }
}

void Dialogue::update( float /* deltaTime */ )
{
    if ( ! this->is_enabled() )
    {
        return;
    }

    ImGui::P_Show( "Dialogue Editor", &m_showCustomisation, [&] {
        sf::Color background { m_shape.getFillColor() };
        sf::Color outline { m_shape.getOutlineColor() };
        float     outlineThickness { m_shape.getOutlineThickness() };

        unsigned int characterSize { m_text.getCharacterSize() };
        sf::Color    characterColor { m_text.getFillColor() };

        ImGui::P_ColorEditor( "Background", background );
        ImGui::P_ColorEditor( "Outline", outline );
        ImGui::InputFloat( "Outline Thickness", &outlineThickness );

        ImGui::P_InputNumber( "Character Size", characterSize );
        ImGui::P_ColorEditor( "Character Color", characterColor );

        m_shape.setFillColor( background );
        m_shape.setOutlineColor( outline );
        m_shape.setOutlineThickness( outlineThickness );

        m_text.setCharacterSize( characterSize );
        m_text.setFillColor( characterColor );

        std::stringstream stream {};
        stream << "Position : '" << m_shape.getPosition() << "'\n";
        stream << "Size : '" << m_shape.getSize() << "'\n";
        stream << "Text Remaining : '" << m_textRemaining << "'\n";
        stream << "Current Text : '" << m_text.getString().toAnsiString()
               << "'\n";
        ImGui::Text( "%s", stream.str().c_str() );
    } );
}

void Dialogue::render( Render & render ) const
{
    if ( ! this->is_enabled() )
    {
        return;
    }

    render.get_target().draw( m_shape, render.get_state() );
    render.get_target().draw( m_text, render.get_state() );
}

void Dialogue::set_current_text( std::string const & text )
{
    m_text.setString( text );
    // The origin has an offset (value of the left and top local bounds)
    m_text.setOrigin(
        m_text.getLocalBounds().left, m_text.getLocalBounds().top );

    sf::Vector2f const textSize {
        m_text.getLocalBounds().width, m_text.getLocalBounds().height };
    // The position of the text must be at the middle of the button shape
    m_text.setPosition(
        m_shape.getPosition() + ( ( m_shape.getSize() - textSize ) / 2.f ) );
}

void Dialogue::enable()
{
    m_isEnabled = true;
}

void Dialogue::disable()
{
    m_isEnabled = false;
}

bool Dialogue::is_enabled() const
{
    return m_isEnabled;
}