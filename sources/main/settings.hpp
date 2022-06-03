#pragma once

#include "graphics2D/sfml.hpp"
#include "tools/maths.hpp"
#include "tools/singleton.hpp"

class Settings final : public Singleton< Settings >
{
    friend Settings & Singleton< Settings >::get_instance();

  public:
    virtual ~Settings() = default;

    math::Vector2D get_video_mode() const;
    double get_refresh_rate() const;
    bool get_vertical_sync() const;

  private:
    Settings();

    math::Vector2D m_windowSize;
    double m_refreshRate;
    bool m_verticalSync;
};
