#include "button.hpp"

#include <SFML/Graphics/Color.hpp>         // for Color, Color::Black, Color...
#include <SFML/Graphics/Rect.hpp>          // for FloatRect
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget

namespace sf
{

    class Font;
}  // namespace sf

Button::Button( sf::Font const &    font,
                std::string const & string /* = "Button" */ )
  : m_shape(), m_text()
{
    // Button default property
    this->setPosition( 0.f, 0.f );

    // Initalize Shape
    this->m_shape.setFillColor( sf::Color::White );
    this->m_shape.setOutlineColor( sf::Color::Black );
    this->m_shape.setOutlineThickness( 1.f );

    // Initalize text
    this->m_text.setFont( font );
    this->m_text.setFillColor( sf::Color::Black );

    this->set_string( string );
}

sf::Vector2f Button::get_size() const
{
    return this->m_shape.getSize();
}

void Button::set_size( unsigned int const & characterSize )
{
    this->m_text.setCharacterSize( characterSize );
    this->synchronize_string();
}

// void Button::set_size( sf::Vector2f const & size )
// {
//     this->setScale( size / this->get_size() );
//     // this->set_string( this->m_text.getString() );
// }

// void Button::set_size( float const & sizeX, float const & sizeY )
// {
//     this->set_size( sf::Vector2f { sizeX, sizeY } );
// }

void Button::set_string( std::string const & string )
{
    this->m_text.setString( string );
    this->synchronize_string();
}

void Button::synchronize_string()
{
    // The origin has an offset (value of the left and top local bounds)
    this->m_text.setOrigin( this->m_text.getLocalBounds().left,
                            this->m_text.getLocalBounds().top );

    sf::Vector2f const textSize { this->m_text.getLocalBounds().width,
                                  this->m_text.getLocalBounds().height };

    // Scale of the shape in relation of size of text
    float const factorSize { 1.8f };
    // Adapt size of the shape according to the size of the text
    this->m_shape.setSize( textSize * factorSize );

    // The position of the text must be at the middle of the button shape
    this->m_text.setPosition( this->getPosition()
                              + ( ( this->get_size() - textSize ) / 2.f ) );
}

void Button::set_selected( bool const & isSelected )
{
    sf::Color buttonColor { this->m_shape.getFillColor() };

    // Increase or decrease transparency if it's selected or not
    buttonColor.a = ( isSelected ) ? 128 : 255;

    this->m_shape.setFillColor( buttonColor );
}

void Button::set_pressed( bool const & isPressed )
{
    sf::Color buttonColor { this->m_shape.getFillColor() };

    // Increase or decrease transparency if it's pressed or not
    buttonColor.a = ( isPressed ) ? 192 : 255;

    this->m_shape.setFillColor( buttonColor );
}

bool Button::is_inside( sf::Vector2f const & position ) const
{
    return ( position.x >= this->getPosition().x
             && position.x <= this->getPosition().x + this->get_size().x
             && position.y >= this->getPosition().y
             && position.y <= this->getPosition().y + this->get_size().y );
}

bool Button::update_button( sf::Vector2f const & position, bool const & click )
{
    if ( this->is_inside( position ) )
    {
        this->set_selected( true );

        if ( click )
        {
            this->set_pressed( true );
            return true;
        }

        this->set_pressed( false );
        return false;
    }

    this->set_selected( false );
    this->set_pressed( false );
    return false;
}

void Button::render_before( RenderContext & context ) const
{
    context.draw( m_shape );
    context.draw( m_text );
}
