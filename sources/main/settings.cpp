#include "settings.hpp"

#include <fstream>

#include "tools/exceptions.hpp"
#include "tools/string.hpp"
#include "tools/tools.hpp"

Settings::Settings() : m_windowSize(), m_refreshRate(), m_verticalSync()
{
    // TYPO Récupéré les settings à partir de la base de données
    std::string const configLocation { tools::get_path::resources()
                                       + "/window.txt"s };

    std::ifstream file { configLocation, std::ios::in };
    // TYPO if we don't find the file, we must create it and load default ressources
    if ( ! file )
    {
        throw Exception::FileNotFound { configLocation };
    }

    unsigned int framePerSecond;
    file >> this->m_windowSize.x >> this->m_windowSize.y >> this->m_verticalSync
        >> framePerSecond;

    this->m_refreshRate = 1. / framePerSecond;
}

math::Vector2D Settings::get_video_mode() const
{
    return this->m_windowSize;
}

double Settings::get_refresh_rate() const
{
    return this->m_refreshRate;
}
bool Settings::get_vertical_sync() const
{
    return this->m_verticalSync;
}