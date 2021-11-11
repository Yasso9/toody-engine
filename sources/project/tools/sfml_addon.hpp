#pragma once

#include <project/utility/utility.hpp>

// Function improve readability of the use of SFML Library

namespace sfaddon
{
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u ) noexcept;

    sf::Vector2u window_size_u() noexcept;

    sf::Vector2f window_size_f() noexcept;
} // namespace sfaddon
