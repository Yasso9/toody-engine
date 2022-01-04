#pragma once

#include <project/tools/sfml.hpp>

class Settings final
{
  public:
    Settings();

    Settings( const Settings & ) noexcept = default;
    Settings( Settings && ) noexcept = delete;
    Settings & operator=( const Settings & ) = delete;
    Settings & operator=( Settings && ) noexcept = delete;

    virtual ~Settings() noexcept = default;

    sf::Vector2u get_window_size_u() const noexcept;
    sf::Vector2f get_window_size_f() const noexcept;
    double get_refresh_rate() const noexcept;
    bool get_vertical_sync() const noexcept;

  private:
    sf::Vector2u m_windowSize {};
    double m_refreshRate {};
    bool m_verticalSync {};
};
