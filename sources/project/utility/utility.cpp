#include "utility.hpp"

void secure_entry( unsigned int & size )
{
    while ( ! ( std::cin >> size ) || size > 500 )
    {
        if ( std::cin.eof() )
        {
            throw std::runtime_error( "Le flux a été fermé !" );
        }
        else if ( std::cin.fail() )
        {
            std::cout << "Entrée invalide. Recommence." << std::endl;
            std::cin.clear();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(),
                             '\n' );
        }
        else
        {
            std::cout << "Size must be an integer between 1 and 500"
                      << std::endl;
        }
    }
}

std::ostream & operator<<( std::ostream & flux, sf::Vector2f const & vector )
{
    return flux << "|x : " << vector.x << " and y : " << vector.y << "|";
}

std::ostream & operator<<( std::ostream & flux, sf::Vector2u const & vector )
{
    return flux << "|x : " << vector.x << " and y : " << vector.y << "|";
}

sf::Vector2f get_movement( Direction const direction, float const value )
{
    sf::Vector2f moving { 0.f, 0.f };

    switch ( direction )
    {
    case Direction::Up :
        moving.y = -value;
        break;
    case Direction::Down :
        moving.y = value;
        break;
    case Direction::Right :
        moving.x = value;
        break;
    case Direction::Left :
        moving.x = -value;
        break;
    default :
        throw std::invalid_argument( "default use in get_movement" );
        break;
    }

    return moving;
}

sf::Vector2f to_view( sf::Vector2f const & positionInLastView,
                      sf::View const & lastView, sf::View const & newView )
{
    /* positionInLastView -> convertion dans la nouvelle vue -> positionInNewView
    -> rajout du départ de la nouvelle vue par rapport à l'ancienne ->
    positionFinish */

    return sf::Vector2f(
        ( positionInLastView.x
          * ( newView.getSize().x / lastView.getSize().x ) )
            + ( lastView.getCenter().x - ( newView.getSize().x / 2.f ) ),
        ( positionInLastView.y
          * ( newView.getSize().y / lastView.getSize().y ) )
            + ( lastView.getCenter().y - ( newView.getSize().y / 2.f ) ) );
}