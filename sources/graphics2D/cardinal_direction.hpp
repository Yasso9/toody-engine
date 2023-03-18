#pragma once

#include <string>

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