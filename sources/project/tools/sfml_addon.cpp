#include "sfml_addon.hpp"

namespace sfaddon
{
    sf::VideoMode to_video_mode( sf::Vector2u const & vector2u ) noexcept
    {
        return sf::VideoMode { vector2u.x, vector2u.y };
    }

    sf::Vector2u window_size_u() noexcept
    {
        sf::VideoMode const videoMode { sf::VideoMode::getDesktopMode() };
        return sf::Vector2u { videoMode.width, videoMode.height };
    }

    sf::Vector2f window_size_f() noexcept
    {
        return static_cast<sf::Vector2f>( window_size_u() );
    }
} // namespace sfaddon