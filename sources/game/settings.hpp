#pragma once

#include <filesystem>

#include <SFML/Window/VideoMode.hpp>  // for VideoMode
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include "libraries/sub_window.hpp"  // for SubWindow
#include "maths/vector2.hpp"         // for Vector2F
#include "tools/singleton.hpp"       // for Singleton, ENABLE_SINGLETON

enum class StateList;

class Settings : public Singleton< Settings >,
                 public SubWindow
{
    ENABLE_SINGLETON( Settings );

    // Settings file path
    std::filesystem::path m_filePath;

    math::Vector2F m_windowSize;
    float          m_nbFramePerSecond;
    bool           m_verticalSync;
    StateList      m_startupState;
    float          m_uiScale;
    float          m_fontScale;

    Settings();

  public:
    ~Settings() = default;

    void update_gui () override;

    math::Vector2F get_window_size () const;
    sf::VideoMode  get_video_mode () const;
    float          get_refresh_rate () const;
    bool           get_vertical_sync () const;
    StateList      get_startup_state () const;
    float          get_ui_scale () const;
    float          get_font_scale () const;

  private:
    // load the default configuration
    void load_default ();
    // load settings from file
    void load ();
    // save settings to file
    void save () const;

    BOOST_DESCRIBE_CLASS( Settings, (), (), (),
                          ( m_windowSize, m_nbFramePerSecond, m_verticalSync,
                            m_startupState, m_uiScale, m_fontScale ) )
};
