#include "configuration.hpp"

#include <filesystem>  // for operator/, path
#include <fstream>     // for basic_istream<>::__istream_type, ifs...
#include <string>      // for string, operator""s

#include "application/resources.hpp"  // for resources
#include "states/state.hpp"           // for StateList
#include "tools/enum.hpp"             // for operator>>, operator<<
#include "tools/traces.hpp"           // for FileIssue

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

Config::Config()
  : SubWindow { "Settings" },
    m_filePath { resource::app_data::SETTINGS },
    m_windowSize {},
    m_nbFramePerSecond {},
    m_verticalSync {},
    m_startupState {},
    m_uiScale {},
    m_fontScale {},
    m_tilemapSavePath {}
{
    this->load();
}

math::Vector2F Config::get_window_size() const
{
    return m_windowSize;
}

sf::VideoMode Config::get_video_mode() const
{
    math::Vector2U windowSizeUInt { m_windowSize.to_u_int() };
    return sf::VideoMode { windowSizeUInt.x, windowSizeUInt.y };
}

float Config::get_refresh_rate() const
{
    return 1.f / m_nbFramePerSecond;
}

bool Config::get_vertical_sync() const
{
    return m_verticalSync;
}

StateList Config::get_startup_state() const
{
    return m_startupState;
}

float Config::get_ui_scale() const
{
    return m_uiScale;
}

float Config::get_font_scale() const
{
    return m_fontScale;
}

std::filesystem::path Config::get_tilemap_save_path() const
{
    return m_tilemapSavePath;
}

void Config::update_gui()
{
    if ( ImGui::BeginWindow( *this ) )
    {
        unsigned int aligment = 25;

        ImGui::TextFmt( "{:<{}}", "Window size", aligment );
        ImGui::SameLine();
        ImGui::InputVec( "##WindowSize", m_windowSize, "%.0f" );

        ImGui::TextFmt( "{:<{}}", "Frame rate", aligment );
        ImGui::SameLine();
        if ( ImGui::InputFloat( "##Framerate", &m_nbFramePerSecond, 1.f, 5.f,
                                "%.0f" ) )
        {
            if ( m_nbFramePerSecond < 5.f )
            {
                m_nbFramePerSecond = 5.f;
            }
        }

        ImGui::TextFmt( "{:<{}}", "Vertical Sync", aligment );
        ImGui::SameLine();
        ImGui::Checkbox( "##VerticalSync", &m_verticalSync );

        ImGui::TextFmt( "{:<{}}", "Startup State", aligment );
        ImGui::SameLine();
        std::vector< std::string > stateList     = get_list< StateList >();
        std::string                selectedState = to_string( m_startupState );
        if ( ImGui::BeginCombo( "##Startup State Combo",
                                selectedState.c_str() ) )
        {
            for ( std::string const & state : stateList )
            {
                bool isSelected = ( state == selectedState );
                if ( ImGui::Selectable( state.c_str(), isSelected ) )
                {
                    m_startupState = to_enum< StateList >( state );
                }
                // Set the initial focus when opening the combo (scrolling +
                // keyboard navigation focus)
                if ( isSelected )
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::TextFmt( "{:<{}}", "UI Scale", aligment );
        ImGui::SameLine();
        if ( ImGui::InputFloat( "##UIScale", &m_uiScale, 0.5f, 1.f, "%.1f" ) )
        {
            if ( m_uiScale < 0.5f )
            {
                m_uiScale = 0.5f;
            }
            else if ( m_uiScale > 4.f )
            {
                m_uiScale = 4.f;
            }
        }

        ImGui::TextFmt( "{:<{}}", "Font Scale", aligment );
        ImGui::SameLine();
        if ( ImGui::InputFloat( "##FontScale", &m_fontScale, 0.5f, 1.f,
                                "%.1f" ) )
        {
            if ( m_fontScale < 0.5f )
            {
                m_fontScale = 0.5f;
            }
            else if ( m_fontScale > 4.f )
            {
                m_fontScale = 4.f;
            }
        }

        if ( ImGui::Button( "Load Default" ) )
        {
            this->load_default();
        }
        ImGui::SameLine();
        if ( ImGui::Button( "Save" ) )
        {
            this->save();
        }
        ImGui::Text( "* Restart the game to apply changes" );
    }
    ImGui::End();
}

void Config::load_default()
{
    m_windowSize       = { 1600.f, 900.f };
    m_nbFramePerSecond = 60.f;
    m_verticalSync     = true;
    m_startupState     = StateList::MainMenu;
    m_uiScale          = 1.f;
    m_fontScale        = 1.f;
    m_tilemapSavePath  = resource::app_data::get_path( "tilemap.txt" );
}

void Config::load()
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
        Config, boost::describe::mod_any_access > >( [&, this] ( auto D ) {
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
        Trace::Error( "Settings file contains to much parameters '{}'",
                      m_filePath.string() );
        error = true;
    }

    if ( error )
    {
        this->load_default();
        this->save();
    }
}

void Config::save() const
{
    std::ofstream file { m_filePath, std::ios::out | std::ios::trunc };
    if ( ! file )
    {
        Trace::FileIssue( m_filePath, "Can't write" );
    }

    // List of all members of Settings
    boost::mp11::mp_for_each< boost::describe::describe_members<
        Config, boost::describe::mod_any_access > >( [&, this] ( auto D ) {
        file << this->*D.pointer << "\n";
        if ( ! file )
        {
            Trace::Error( "Failed to write in '{}'", m_filePath.string() );
        }
    } );
}
