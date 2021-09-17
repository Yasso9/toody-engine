#include "information.hpp"

Message::Message()
{

}


void Message::create( sf::Font const & font, unsigned int const textNumber )
{
    sf::Text baseText {};
    baseText.setFont( font );
    baseText.setCharacterSize( 16 );
    // baseText.setScale(0.3, 0.3);
    baseText.setScale( 1.f, 1.f );
    baseText.setLetterSpacing( 1.f );
    baseText.setFillColor( sf::Color::Blue );
    baseText.setStyle( sf::Text::Regular );

    for ( unsigned int i_textNum { 0u }; i_textNum < textNumber; ++i_textNum )
    {
        this->m_textArray.push_back( baseText );
    }
}

void Message::update( sf::Vector2f const position, std::vector<std::string> const & message )
{
    assert( message.size() == this->m_textArray.size() &&
    "Number of argument not comptatible with this variable of the class Information" );

    for ( unsigned int i_text { 0u }; i_text < this->m_textArray.size(); ++i_text )
    {
        this->m_textArray[i_text].setPosition(
            position
            + sf::Vector2f(
                0.f,
                static_cast<float>(i_text * this->m_textArray[i_text].getCharacterSize() * 1.5)
            )
        );

        this->m_textArray[i_text].setString( message[i_text] );
    }
}

void Message::render( sf::RenderWindow & target ) const
{
    for ( unsigned int i_text { 0u }; i_text < this->m_textArray.size(); ++i_text )
    {
        target.draw( this->m_textArray[i_text] );
    }
}




TileCursor::TileCursor()
{
    this->setPosition( 0.f, 0.f );
    this->setSize( sf::Vector2f( g_squareSize, g_squareSize ) );
    this->setFillColor( sf::Color( 20, 20, 200, 128 ) );
    // this->shape.setOutlineThickness(7.f);
    // this->shape.setOutlineColor(sf::Color(100, 100, 255, 200));
}

void TileCursor::update( sf::Vector2f const cursorPosition, sf::Vector2f const mapBeginPosition )
{
    this->setPosition(
        sf::Vector2f( to_tile_position( cursorPosition - mapBeginPosition ) )
        * g_squareSize + mapBeginPosition
    );
}