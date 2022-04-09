#include "settings.hpp"

#include <fstream>

#include "tools/exceptions.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

Settings::Settings()
{
    // TYPO mettre le windows.txt dans un autre endroit que resources
    std::string const configLocation { tools::get_path::resources()
                                       + "/window.txt"s };

    std::ifstream file { configLocation, std::ios::in };
    if ( ! file )
    {
        throw Exception::FileNotFound { configLocation };
    }

    unsigned int framerate;
    file >> this->m_windowSize.x >> this->m_windowSize.y >> this->m_verticalSync
        >> framerate;

    this->m_refreshRate = 1. / framerate;
}

sf::Vector2u Settings::get_window_size_u() const
{
    return this->m_windowSize;
}
sf::Vector2f Settings::get_window_size_f() const
{
    return static_cast< sf::Vector2f >( this->get_window_size_u() );
}

double Settings::get_refresh_rate() const
{
    return this->m_refreshRate;
}
bool Settings::get_vertical_sync() const
{
    return this->m_verticalSync;
}