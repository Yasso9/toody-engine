#include "cardinal_direction.hpp"

#include <iostream>

std::string to_string ( E_Direction direction )
{
    switch ( direction )
    {
    case E_Direction::Up :
        return "Up";
        break;
    case E_Direction::Down :
        return "Down";
        break;
    case E_Direction::Right :
        return "Right";
        break;
    case E_Direction::Left :
        return "Left";
        break;
    default :
        std::cerr << "Unkown enumeration for E_Direction" << std::endl;
        return "[Unknown]";
        break;
    }
}
