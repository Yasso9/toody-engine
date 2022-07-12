#include "settings.hpp"

#include <fstream>

#include "tools/exceptions.hpp"
#include "tools/path.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

Settings::Settings() : m_windowSize(), m_refreshRate(), m_verticalSync()
{
    /// @todo Récupéré les settings à partir de la base de données
    std::string const configLocation {
        ( path::get_folder( path::E_Folder::Data ) / "window.txt"s ).string()
    };

    std::ifstream file { configLocation, std::ios::in };
    /// @todo if we don't find the file, we must create it and load default ressources
    if ( ! file )
    {
        throw exception::FileNotFound { configLocation };
    }

    unsigned int framePerSecond;
    file >> this->m_windowSize.x >> this->m_windowSize.y >> this->m_verticalSync
        >> framePerSecond;

    this->m_refreshRate = 1. / framePerSecond;
}

sf::VideoMode Settings::get_video_mode() const
{
    return sf::VideoMode { static_cast< unsigned int >( this->m_windowSize.x ),
                           static_cast< unsigned int >(
                               this->m_windowSize.y ) };
}

double Settings::get_refresh_rate() const
{
    return this->m_refreshRate;
}
bool Settings::get_vertical_sync() const
{
    return this->m_verticalSync;
}