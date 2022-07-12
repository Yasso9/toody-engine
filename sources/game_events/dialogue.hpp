#pragma once

#include <string>

#include "graphics2D/sfml.hpp"
#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "main/window.hpp"
#include "maths/maths.hpp"

class Dialogue
{
  private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    bool m_showCustomisation;

  public:
    Dialogue() : m_shape(), m_text(), m_showCustomisation( true )
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

        m_text.setFont(
            Resources::get_instance().get_font( Resources::E_FontKey::Arial ) );
        m_text.setCharacterSize( 30u );
        m_text.setFillColor( sf::Color::Black );

        this->set_text( "" );
    }
    virtual ~Dialogue() = default;

    void update_customisation()
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

    void show() const
    {
        Window::get_instance().sf_draw( m_shape );
        Window::get_instance().sf_draw( m_text );
    }

  private:
    void set_text( std::string const & text )
    {
        m_text.setString( text );
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