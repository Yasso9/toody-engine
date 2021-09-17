#include "tileset.hpp"

// TYPO trouver une meilleurs place à ce convertisseur
inline sf::Vector2f to_Vector2f( sf::Rect<int> const & intRectangle )
{
    return sf::Vector2f {
        static_cast<float>(intRectangle.width),
        static_cast<float>(intRectangle.height)
    };
}

Tileset::Tileset()
{
    this->isMouseOn = false;
    this->isPrint = false;

    this->setOrigin( sf::Vector2f { 0.f, 0.f } );
    this->setPosition( sf::Vector2f { 0.f, 0.f } );

    this->setOutlineThickness( 1.f );
    this->setOutlineColor( sf::Color { 20, 20, 20, 50 } );
    this->setFillColor( sf::Color { 255, 255, 255, 200 } );
}


void Tileset::create( sf::Texture const & texture )
{
    // true sert a mettre automatiquement setTextureRect 
    // en fonction de la taille de la texture
    this->setTexture( &texture, true );
    this->setSize( to_Vector2f( this->getTextureRect() ) );

    // A mettre après avoir changer la texture
    // assert(
    //     this->getTextureRect().width.x % g_squareSize == 0 &&
    //     this->getTextureRect().height.y % g_squareSize == 0 &&
    //     "La taille du tileset n'est pas un multiple de 32"
    // );
}

void Tileset::update_tile_cursor( sf::Vector2f const cursorPosition )
{
    this->m_tileCursor.update( cursorPosition, this->getPosition() );
}

bool Tileset::include( sf::Vector2f const position ) const
{
    return is_in_part(
        position,
        this->getPosition(),
        to_Vector2f( this->getTextureRect() )
    );
}

void Tileset::update( sf::Vector2f const mousePosition, unsigned int & tile,
bool const buttonIsPress )
{
    if ( this->isPrint && this->include( mousePosition ) )
    {
        this->isMouseOn = true;

        this->update_tile_cursor( mousePosition );

        if ( buttonIsPress )
        {
            sf::Vector2u const tilesetPosition {
                to_tile_position(
                    to_Vector2f( this->getTextureRect() ) + mousePosition - this->getPosition()
                )
            };

            tile = tilesetPosition.x
                + tilesetPosition.y
                * static_cast<unsigned int>(this->getTextureRect().width)
                / static_cast<unsigned int>(g_squareSize);
        }
    }
    else
    {
        this->isMouseOn = false;
    }
}

void Tileset::render( sf::RenderWindow & target ) const
{
    if ( this->isPrint )
    {
        target.draw( *this );

        if ( this->isMouseOn )
        {
            target.draw( this->m_tileCursor );
        }
    }
}