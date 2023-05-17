#pragma once

#include <filesystem>

#include <SFML/Window/VideoMode.hpp>  // for VideoMode
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include "maths/vector2.hpp"    // for Vector2F
#include "tools/singleton.hpp"  // for Singleton

class Settings : public Singleton< Settings >
{
    ENABLE_SINGLETON( Settings );

    std::filesystem::path m_filePath;

    math::Vector2F m_windowSize;
    float          m_nbFramePerSecond;
    bool           m_verticalSync;

    Settings();

  public:
    virtual ~Settings() = default;

    math::Vector2F get_window_size () const;
    sf::VideoMode  get_video_mode () const;
    float          get_refresh_rate () const;
    bool           get_vertical_sync () const;

  private:
    /// @brief load a default configuration
    void load_default ();
    /// @brief load settings from file
    void load ();
    /// @brief save settings to file
    void save () const;

    BOOST_DESCRIBE_CLASS( Settings, (), (), (),
                          ( m_windowSize, m_nbFramePerSecond, m_verticalSync ) )
};
