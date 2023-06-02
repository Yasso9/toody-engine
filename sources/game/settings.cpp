#include "settings.hpp"

#include <filesystem>  // for operator/, path
#include <fstream>     // for basic_istream<>::__istream_type, ifs...
#include <string>      // for string, operator""s

#include "tools/enum.hpp"    // for operator>>, operator<<
#include "tools/path.hpp"    // for get_folder, E_Folder, E_Folder::Data
#include "tools/traces.hpp"  // for FileIssue

// TODO have this function available globally
template< typename T,
          class Bd = boost::describe::describe_bases<
              T, boost::describe::mod_any_access >,
          class Md = boost::describe::describe_members<
              T, boost::describe::mod_any_access > >
std::string get_description ( T const & value, char separator = '\n' )
{
    std::ostringstream os;

    boost::mp11::mp_for_each< Bd >( [&] ( auto D ) {
        using B = typename decltype( D )::type;
        os << static_cast< B const & >( value ) << "\n";
    } );
    boost::mp11::mp_for_each< Md >( [&] ( auto D ) {
        os << "." << D.name << " = " << value.*D.pointer << separator;
    } );

    return os.str();
}

Settings::Settings()
  : SubWindow { "Settings" },
    m_filePath { path::get_folder( path::Data ) / "settings.txt" },
    m_windowSize {},
    m_nbFramePerSecond {},
    m_verticalSync {},
    m_startupState {}
{
    this->load();
}

math::Vector2F Settings::get_window_size() const
{
    return m_windowSize;
}

sf::VideoMode Settings::get_video_mode() const
{
    math::Vector2U windowSizeUInt { m_windowSize.to_u_int() };
    return sf::VideoMode { windowSizeUInt.x, windowSizeUInt.y };
}

float Settings::get_refresh_rate() const
{
    return 1.f / m_nbFramePerSecond;
}

bool Settings::get_vertical_sync() const
{
    return m_verticalSync;
}

void Settings::update_gui_window()
{
    if ( ImGui::BeginWindow( *this ) )
    {
        ImGui::Text( "Window size" );
        ImGui::SameLine();
        ImGui::InputFloat( "Width", &m_windowSize.x );
        ImGui::InputFloat( "Height", &m_windowSize.y );

        ImGui::Text( "Frame rate" );
        ImGui::InputFloat( "Frame rate", &m_nbFramePerSecond );

        ImGui::Text( "Vertical sync" );
        ImGui::Checkbox( "Vertical sync", &m_verticalSync );

        ImGui::Text( "Startup state" );
        // Dropdown list of State::E_List
        // ImGui::Combo( "Startup state" );

        if ( ImGui::Button( "Load Default" ) )
        {
            this->load_default();
        }
        ImGui::SameLine();
        if ( ImGui::Button( "Save" ) )
        {
            this->save();
        }
    }
    ImGui::End();
}

void Settings::load_default()
{
    m_windowSize       = { 800.f, 600.f };
    m_nbFramePerSecond = 60.f;
    m_verticalSync     = true;
    m_startupState     = State::MainMenu;
}

void Settings::load()
{
    std::ifstream file { m_filePath, std::ios::in };
    if ( ! file )
    {
        Trace::FileIssue( m_filePath, "Can't read" );
        this->load_default();
        this->save();
        return;
    }

    bool error = false;
    // List of all members of Settings
    boost::mp11::mp_for_each< boost::describe::describe_members<
        Settings, boost::describe::mod_any_access > >( [&, this] ( auto D ) {
        if ( file.peek() == std::ifstream::traits_type::eof() )
        {
            Trace::Error( "Unexpected end of file '{}'", m_filePath.string() );
            error = true;
        }
        file >> this->*D.pointer;
        if ( ! file )
        {
            Trace::Error( "Failed to read '{}'", m_filePath.string() );
            error = true;
        }
        // Trace::Debug( "Settings : {} -> {}", this->*D.pointer, D.name );
    } );

    if ( ! stream::is_ended( file ) )
    {
        Trace::Error( "Settings file to much parameters '{}'",
                      m_filePath.string() );
        error = true;
    }

    if ( error )
    {
        this->load_default();
        this->save();
    }
}

void Settings::save() const
{
    std::ofstream file { m_filePath, std::ios::out | std::ios::trunc };
    if ( ! file )
    {
        Trace::FileIssue( m_filePath, "Can't write" );
    }

    // List of all members of Settings
    boost::mp11::mp_for_each< boost::describe::describe_members<
        Settings, boost::describe::mod_any_access > >( [&, this] ( auto D ) {
        file << this->*D.pointer << "\n";
    } );
}
