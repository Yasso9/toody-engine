#include "button.hpp"

#include <project/tools/string.hpp>

Button::Button( sf::Font const & font ) : m_shape(), m_text()
{
    this->setPosition( 0.f, 0.f );

    sf::Vector2f const basicSize { 50.f, 50.f };

    this->m_shape.setSize( basicSize );
    this->m_shape.setFillColor( sf::Color::White );
    this->m_shape.setOutlineColor( sf::Color::Black );
    this->m_shape.setOutlineThickness( 2.f );

    this->m_text.setFont( font );
    this->m_text.setCharacterSize( 20 );
    this->m_text.setFillColor( sf::Color::Black );

    this->set_string( "Button"s );
}

sf::Vector2f Button::get_size() const noexcept
{
    return this->m_shape.getSize() * this->getScale();
}

void Button::set_size( sf::Vector2f const & size ) noexcept
{
    this->setScale( size / this->get_size() );
}

void Button::set_size( float const & sizeX, float const & sizeY ) noexcept
{
    this->set_size( sf::Vector2f { sizeX, sizeY } );
}

void Button::set_string( std::string const & string )
{
    this->m_text.setString( string );

    // TYPO ? Global Local
    this->m_text.setOrigin(
        this->m_text.getLocalBounds().left
            + ( this->m_text.getLocalBounds().width / 2 ),
        this->m_text.getLocalBounds().top
            + ( this->m_text.getGlobalBounds().height / 2 ) );

    this->m_text.setPosition( this->getPosition() + this->get_size() / 2.f );
}

void Button::set_selected( bool const & isSelected ) noexcept
{
    sf::Color shapeColor { this->m_shape.getFillColor() };

    shapeColor.a = ( isSelected ) ? 128 : 255;

    this->m_shape.setFillColor( shapeColor );
}

void Button::set_pressed( bool const & isPressed ) noexcept
{
    sf::Color shapeColor { this->m_shape.getFillColor() };

    shapeColor.a = ( isPressed ) ? 192 : 255;

    this->m_shape.setFillColor( shapeColor );
}

bool Button::is_inside( sf::Vector2f const & position ) const noexcept
{
    return ( position.x >= this->getPosition().x
             && position.x <= this->getPosition().x + this->get_size().x
             && position.y >= this->getPosition().y
             && position.y <= this->getPosition().y + this->get_size().y );
}

bool Button::update( sf::Vector2f const & position, bool const & click )
{
    // Initialisation of the selection
    this->set_selected( false );
    this->set_pressed( false );

    if ( this->is_inside( position ) )
    {
        this->set_selected( true );

        if ( click == true )
        {
            this->set_pressed( true );
            return true;
        }
    }

    return false;
}

void Button::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    target.draw( this->m_shape, states );
    target.draw( this->m_text, states );
}