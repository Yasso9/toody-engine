#pragma once

#include "tools/sfml.hpp"

class Settings final
{
  public:
    Settings();
    virtual ~Settings() = default;

    sf::Vector2u get_window_size_u() const;
    sf::Vector2f get_window_size_f() const;
    double get_refresh_rate() const;
    bool get_vertical_sync() const;

  private:
    sf::Vector2u m_windowSize;
    double m_refreshRate;
    bool m_verticalSync;
};
