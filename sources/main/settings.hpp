#pragma once

#include <SFML/Window/VideoMode.hpp> // for VideoMode

#include "maths/vector2.hpp"   // for Vector2F
#include "tools/singleton.hpp" // for Singleton

class Settings final : public Singleton< Settings >
{
    friend Settings & Singleton< Settings >::get_instance();

  public:
    virtual ~Settings() = default;

    sf::VideoMode get_video_mode() const;
    double get_refresh_rate() const;
    bool get_vertical_sync() const;

  private:
    Settings();

    math::Vector2F m_windowSize;
    double m_refreshRate;
    bool m_verticalSync;
};
