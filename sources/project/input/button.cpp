#include "button.hpp"

Button::Button()
{
    this->m_shape.setFillColor( sf::Color::White );
    this->m_shape.setSize( sf::Vector2f( 200.f, 100.f ) );
    this->m_shape.setPosition( 0.f, 0.f );
    this->m_shape.setOutlineColor( sf::Color::Black );
    this->m_shape.setOutlineThickness( 2.f );

    this->set_string( "Button" );
}

Button::Button( sf::Font const & font ) : Button()
{
    this->m_text.setFont( font );
    this->m_text.setCharacterSize( 20 );
    this->m_text.setFillColor( sf::Color::Black );
}

Button::Button( sf::Font const & font, std::string const & buttonString )
  : Button( font )
{
    this->set_string( buttonString );
}

void Button::set_position( sf::Vector2f const & position )
{
    this->m_shape.setPosition( position );
    this->adjust_text_position();
}

void Button::set_position( float const & positionX, float const & positionY )
{
    this->m_shape.setPosition( positionX, positionY );
    this->adjust_text_position();
}

void Button::set_string( std::string const & string )
{
    this->m_text.setString( string );

    // ? Global Local
    this->m_text.setOrigin( this->m_text.getLocalBounds().left
                                + ( this->m_text.getLocalBounds().width / 2 ),
                            this->m_text.getLocalBounds().top
                                + ( this->m_text.getGlobalBounds().height / 2 ) );

    this->adjust_text_position();
}

void Button::adjust_text_position()
{
    this->m_text.setPosition( this->m_shape.getPosition()
                              + this->m_shape.getSize() / 2.f );
}

void Button::set_selected( bool const & isSelected )
{
    sf::Color const shapeColor { this->m_shape.getFillColor() };
    this->m_shape.setFillColor( sf::Color(
        shapeColor.r, shapeColor.g, shapeColor.b, ( isSelected ) ? 128 : 255 ) );

    this->m_isSelected = isSelected;
}

void Button::set_pressed( bool const & isPressed )
{
    sf::Color const shapeColor { this->m_shape.getFillColor() };

    // TYPO mettre la même organisation de code qu'on a mis dans set_selected
    this->m_shape.setFillColor(
        sf::Color( shapeColor.r, shapeColor.g, shapeColor.b, 255 ) );
    if ( isPressed )
    {
        this->m_shape.setFillColor(
            sf::Color( shapeColor.r, shapeColor.g, shapeColor.b, 192 ) );
    }

    this->m_isPressed = isPressed;
}

bool Button::is_inside( sf::Vector2f const & mousePosition ) const
{
    return ( mousePosition.x >= this->m_shape.getPosition().x
             && mousePosition.x
                    <= this->m_shape.getPosition().x + this->m_shape.getSize().x
             && mousePosition.y >= this->m_shape.getPosition().y
             && mousePosition.y
                    <= this->m_shape.getPosition().y + this->m_shape.getSize().y );
}

void Button::draw( sf::RenderTarget & target, sf::RenderStates /* states */ ) const
{
    // states.transform *= m_shape.getTransform();

    target.draw( this->m_shape );
    target.draw( this->m_text );
}