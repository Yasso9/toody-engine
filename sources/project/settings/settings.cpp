#include "settings.hpp"

#include <fstream>

#include <project/tools/exceptions.hpp>
#include <project/tools/string.hpp>

Settings::Settings()
{
    std::string const configLocation { "./ressources/window.txt"s };

    std::ifstream file { configLocation, std::ios::in };
    if ( ! file )
    {
        throw FileNotFoundException { configLocation };
    }

    unsigned int framerate;
    file >> this->m_windowSize.x >> this->m_windowSize.y >> this->m_verticalSync
        >> framerate;

    this->m_refreshRate = 1. / framerate;
}

sf::Vector2u Settings::get_window_size_u() const noexcept
{
    return this->m_windowSize;
}
sf::Vector2f Settings::get_window_size_f() const noexcept
{
    return static_cast<sf::Vector2f>( this->get_window_size_u() );
}

double Settings::get_refresh_rate() const noexcept
{
    return this->m_refreshRate;
}
bool Settings::get_vertical_sync() const noexcept
{
    return this->m_verticalSync;
}