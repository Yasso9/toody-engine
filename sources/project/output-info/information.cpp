#include "information.hpp"

Message::Message( sf::Font const & font ) : m_font( font ) {}

void Message::initialize_text( std::size_t const & size )
{
    this->m_textArray.clear();
    sf::Text baseText {};
    baseText.setFont( this->m_font );
    baseText.setCharacterSize( 16 );
    baseText.setLetterSpacing( 1.f );
    baseText.setFillColor( sf::Color::Black );
    baseText.setStyle( sf::Text::Regular );

    for ( unsigned int i_textNum { 0u }; i_textNum < size; ++i_textNum )
    {
        this->m_textArray.push_back( baseText );
    }
}

void Message::update( sf::Vector2f const position,
                      std::vector<std::string> const & message )
{
    // TYPO ne pas tous reinitialiser mais ajouter ou supprimer si il y'a en plus
    // ou en moins. Et changer les chaines
    this->initialize_text( message.size() );

    unsigned int textIndex { 0u };
    for ( sf::Text & text : this->m_textArray )
    {
        text.setPosition(
            position
            + sf::Vector2f {
                0.f,
                static_cast<float>( textIndex * text.getCharacterSize() )
                    * 1.5f } );
        text.setString( message[textIndex] );

        ++textIndex;
    }
}

void Message::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    for ( unsigned int i_text { 0u }; i_text < this->m_textArray.size();
          ++i_text )
    {
        target.draw( this->m_textArray[i_text], states );
    }
}

Cursor::Cursor() : m_shape()
{
    this->setPosition( 0.f, 0.f );
    this->set_size( 20.f, 20.f );
    this->m_shape.setFillColor( sf::Color( 20, 20, 200, 128 ) );
    // this->m_shape.setOutlineThickness(7.f);
    // this->m_shape.setOutlineColor(sf::Color(100, 100, 255, 200));
}

sf::Vector2f Cursor::get_size() const noexcept
{
    return this->m_shape.getSize() * this->getScale();
}

void Cursor::set_size( sf::Vector2f const & size ) noexcept
{
    this->setScale( size / this->get_size() );
}

void Cursor::set_size( float const & sizeX, float const & sizeY ) noexcept
{
    this->set_size( sf::Vector2f { sizeX, sizeY } );
}

void Cursor::update( sf::FloatRect const & rectangle )
{
    // this->setPosition(
    //     sf::Vector2f( to_tile_position( cursorPosition - mapBeginPosition ) )
    //         * ::g_tileSize_f
    //     + mapBeginPosition );

    this->setPosition( rectangle.left, rectangle.top );
    this->set_size( rectangle.width, rectangle.height );
}

void Cursor::update( sf::Vector2f const & position, sf::Vector2f const & size )
{
    this->update( sf::FloatRect { position, size } );
}

void Cursor::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    target.draw( this->m_shape, states );
}