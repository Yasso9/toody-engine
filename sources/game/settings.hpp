#pragma once

#include <filesystem>

#include <SFML/Window/VideoMode.hpp>  // for VideoMode
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include "libraries/sub_window.hpp"  // for SubWindow
#include "maths/vector2.hpp"         // for Vector2F
#include "states/state.hpp"          // for State::E_List
#include "tools/singleton.hpp"       // for Singleton, ENABLE_SINGLETON

class Settings : public Singleton< Settings >,
                 public SubWindow
{
    ENABLE_SINGLETON( Settings );

    // Settings file path
    std::filesystem::path m_filePath;

    math::Vector2F m_windowSize;
    float          m_nbFramePerSecond;
    bool           m_verticalSync;
    State::E_List  m_startupState;

    Settings();

  public:
    virtual ~Settings() = default;

    math::Vector2F get_window_size () const;
    sf::VideoMode  get_video_mode () const;
    float          get_refresh_rate () const;
    bool           get_vertical_sync () const;
    State::E_List  get_startup_state () const;

    void update_gui_window () override;

  private:
    // load the default configuration
    void load_default ();
    // load settings from file
    void load ();
    // save settings to file
    void save () const;

    BOOST_DESCRIBE_CLASS( Settings, (), (), (),
                          ( m_windowSize, m_nbFramePerSecond, m_verticalSync,
                            m_startupState ) )
};
