#pragma once

#include <SFML/Window/VideoMode.hpp>  // for VideoMode
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include "maths/vector2.hpp"    // for Vector2F
#include "tools/singleton.hpp"  // for Singleton

class Settings
{
  public:
    Settings();
    virtual ~Settings() = default;

    sf::VideoMode get_video_mode () const;
    double        get_refresh_rate () const;
    bool          get_vertical_sync () const;

  private:
    math::Vector2F m_windowSize;
    double         m_refreshRate;
    bool           m_verticalSync;

    BOOST_DESCRIBE_CLASS( Settings, (), (), (),
                          ( m_windowSize, m_refreshRate, m_verticalSync ) )
};
