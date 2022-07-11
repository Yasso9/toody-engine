#pragma once

#include <string>

#include "graphics2D/sfml.hpp"
#include "main/resources.hpp"
#include "main/window.hpp"
#include "maths/maths.hpp"

class Dialogue
{
  private:
    std::string m_text;

  public:
    Dialogue() : m_text( "" ) {}
    virtual ~Dialogue() = default;

    void set_text( std::string const & text ) { m_text = text; }
    void show() const
    {
        sf::RectangleShape shape {};

        math::Vector2F const windowSizeF {
            Window::get_instance().get_size().to_float()
        };

        math::Vector2F dialogueSize { windowSizeF.x * ( 4.f / 5.f ),
                                      windowSizeF.y * ( 1.f / 5.f ) };

        math::Vector2F dialoguePosition {
            ( windowSizeF.x - dialogueSize.x ) / 2.f,
            ( windowSizeF.y - dialogueSize.y ) * 0.8f,
        };

        shape.setPosition( dialoguePosition );
        shape.setSize( dialogueSize );

        Window::get_instance().sf_draw( shape );

        sf::Text text {};

        text.setFont(
            Resources::get_instance().get_font( Resources::E_FontKey::Arial ) );
        text.setCharacterSize( 30u );
        text.setFillColor( sf::Color::Black );
        text.setString( "Main Menu"s );

        // The origin has an offset (value of the left and top local bounds)
        text.setOrigin( text.getLocalBounds().left, text.getLocalBounds().top );
        sf::Vector2f const textSize { text.getLocalBounds().width,
                                      text.getLocalBounds().height };
        // The position of the text must be at the middle of the button shape
        text.setPosition( shape.getPosition()
                          + ( ( shape.getSize() - textSize ) / 2.f ) );
    }
};