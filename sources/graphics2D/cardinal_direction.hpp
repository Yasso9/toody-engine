#pragma once

#include <string>

/// @todo serach for the answer of answered Nov 13, 2018 at 15:13 jtlim
/// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
/// because we want to have to_string as a method of this enum
/// @brief Cardinal Directions
enum class E_Direction
{
    Up = 0,
    Down,
    Right,
    Left,
    EnumLast,
};

std::string to_string ( E_Direction direction );