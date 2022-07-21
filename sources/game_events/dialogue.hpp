#pragma once

#include <string>

#include "graphics2D/component.hpp"
#include "graphics2D/sfml.hpp"
#include "input/input.hpp"
#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "main/window.hpp"
#include "maths/maths.hpp"
#include "tools/exceptions.hpp"

class Dialogue : public Component2D
{
  private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    bool m_showCustomisation;

    std::string m_textRemaining;

    /// @todo create a singleton class where we can access all type of cursor (or set all type of cursor)
    sf::Cursor m_regularCursor;
    sf::Cursor m_moveCursor;

  public:
    /// @ number of character that can be print on a single dialogue box
    unsigned int const CHARACTER_LIMIT { 150u };

    Dialogue()
      : m_shape(),
        m_text(),
        m_showCustomisation( true ),
        m_textRemaining( "" ),
        m_regularCursor(),
        m_moveCursor()
    {
        math::Vector2F const windowSizeF {
            Window::get_instance().get_size().to_float()
        };

        m_shape.setSize(
            { windowSizeF.x * ( 4.f / 5.f ), windowSizeF.y * ( 1.f / 5.f ) } );
        m_shape.setPosition( {
            ( windowSizeF.x - m_shape.getSize().x ) / 2.f,
            ( windowSizeF.y - m_shape.getSize().y ) * 0.8f,
        } );
        m_shape.setFillColor( sf::Color::White );
        m_shape.setOutlineThickness( 3.f );
        m_shape.setOutlineColor( sf::Color::Black );

        m_text.setFont( resources::get_font( "arial.ttf" ) );
        m_text.setCharacterSize( 30u );
        m_text.setFillColor( sf::Color::Black );

        this->set_text( "" );

        if ( ! m_regularCursor.loadFromSystem( sf::Cursor::Arrow ) )
        {
            throw exception::System { "Cannot load regular Cursor" };
        }
        if ( ! m_moveCursor.loadFromSystem( sf::Cursor::SizeAll ) )
        {
            throw exception::System { "Cannot load move Cursor" };
        }
    }
    virtual ~Dialogue() = default;

    /**
     * @brief go to the next dialogue section if available
     * @return true if there is something else to read
     * @return false if there is nothing else to read
     */
    bool next()
    {
        if ( ! m_textRemaining.empty() )
        {
            this->set_text( "" );
        }

        return ! m_textRemaining.empty();
    }

    /// @todo create an inherited class DialogueCustmable that can have process_events_customisation and update_customisation and delete this two functions from this class
    void process_mouse_movement_customisation( math::Vector2I mouseMovement )
    {
        static bool mouseIsInsideBox { false };

        // If the mouse was already inside and we have clicked in the right button
        if ( mouseIsInsideBox && input::is_pressed( sf::Mouse::Right ) )
        {
            this->m_shape.move( mouseMovement.to_float() );
        }

        math::PointI mousePosition { input::get_mouse_position() };
        mouseIsInsideBox = mousePosition.to_float().is_inside(
            math::PointF { this->m_shape.getPosition() },
            this->m_shape.getSize() );

        if ( mouseIsInsideBox )
        {
            Window::get_instance().setMouseCursor( m_moveCursor );
        }
        else
        {
            Window::get_instance().setMouseCursor( m_regularCursor );
        }
    }

    void update_extra( float /* deltaTime */ ) override
    {
        if ( ! m_showCustomisation )
        {
            return;
        }

        if ( ImGui::P_Begin( "Dialogue Editor", &m_showCustomisation ) )
        {
            sf::Color background { m_shape.getFillColor() };
            sf::Color outline { m_shape.getOutlineColor() };
            float outlineThickness { m_shape.getOutlineThickness() };

            unsigned int characterSize { m_text.getCharacterSize() };
            sf::Color characterColor { m_text.getFillColor() };

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
        }
        ImGui::End();
    }

  private:
    void render( Render & render ) const override
    {
        render.draw( m_shape );
        render.draw( m_text );
    }

    void set_text( std::string const & text )
    {
        std::string currentDioalogueText { text };
        if ( text.size() >= CHARACTER_LIMIT )
        {
            m_textRemaining +=
                text.substr( CHARACTER_LIMIT + 1, text.size() - 1 );
            currentDioalogueText = text.substr( 0, CHARACTER_LIMIT );
        }
        else
        {
            currentDioalogueText += m_textRemaining.substr(
                0,
                CHARACTER_LIMIT - currentDioalogueText.size() );
            m_textRemaining.erase(
                0,
                CHARACTER_LIMIT - currentDioalogueText.size() );
        }

        m_text.setString( currentDioalogueText );
        // The origin has an offset (value of the left and top local bounds)
        m_text.setOrigin( m_text.getLocalBounds().left,
                          m_text.getLocalBounds().top );

        sf::Vector2f const textSize { m_text.getLocalBounds().width,
                                      m_text.getLocalBounds().height };
        // The position of the text must be at the middle of the button shape
        m_text.setPosition( m_shape.getPosition()
                            + ( ( m_shape.getSize() - textSize ) / 2.f ) );
    }
};