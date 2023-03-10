#include "settings.hpp"

#include <filesystem>  // for operator/, path
#include <fstream>     // for basic_istream<>::__istream_type, ifs...
#include <string>      // for string, operator""s

#include "tools/path.hpp"  // for get_folder, E_Folder, E_Folder::Data
#include "tools/traces.hpp"

static void print ( Settings const & settings )
{
    std::cout << "{";

    bool first = true;

    boost::mp11::mp_for_each< boost::describe::describe_bases<
        Settings, boost::describe::mod_any_access > >( [&] ( auto D ) {
        if ( ! first )
        {
            std::cout << ", ";
        }
        first = false;

        using B = typename decltype( D )::type;
        std::cout << static_cast< B const & >( settings );
    } );

    boost::mp11::mp_for_each< boost::describe::describe_members<
        Settings, boost::describe::mod_any_access > >( [&] ( auto D ) {
        if ( ! first )
        {
            std::cout << ", ";
        }
        first = false;

        std::cout << "." << D.name << " = " << settings.*D.pointer;
    } );

    std::cout << "}";
}

Settings::Settings() : m_windowSize(), m_refreshRate(), m_verticalSync()
{
    /// @todo Récupéré les settings à partir de la base de données
    std::string const configLocation {
        ( path::get_folder( path::E_Folder::Data ) / "window.txt" ).string() };

    std::ifstream file { configLocation, std::ios::in };
    /// @todo if we don't find the file, we must create it and load default
    /// ressources
    if ( ! file )
    {
        Trace::FileIssue( configLocation, "Can't open the file" );
    }

    unsigned int framePerSecond;
    file >> this->m_windowSize.x >> this->m_windowSize.y >> this->m_verticalSync
        >> framePerSecond;

    this->m_refreshRate = 1. / framePerSecond;

    print( *this );
}

sf::VideoMode Settings::get_video_mode() const
{
    return sf::VideoMode {
        static_cast< unsigned int >( this->m_windowSize.x ),
        static_cast< unsigned int >( this->m_windowSize.y ) };
}

double Settings::get_refresh_rate() const
{
    return this->m_refreshRate;
}

bool Settings::get_vertical_sync() const
{
    return this->m_verticalSync;
}