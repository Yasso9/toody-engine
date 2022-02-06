#pragma once

#include "tools/sfml.hpp"

namespace tools
{
    /** @brief Check if a vector is between the position and position + size */
    bool is_in_part( sf::Vector2f const value, sf::Vector2f const position,
                     sf::Vector2f const size );
} // namespace tools

/// @brief Cardinal Directions
enum class E_Direction
{
    Up = 0,
    Down,
    Right,
    Left,
    EnumLast,
};
