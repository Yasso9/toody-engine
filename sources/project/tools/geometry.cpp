#include "geometry.hpp"

namespace tools
{
    bool is_in_part( sf::Vector2f const value, sf::Vector2f const position,
                     sf::Vector2f const size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }
}