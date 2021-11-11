#pragma once

#include <project/utility/utility.hpp>

class Settings final
{
  public:
    Settings();

    Settings( const Settings & ) = delete;
    Settings( Settings && ) noexcept = delete;
    Settings & operator=( const Settings & ) = delete;
    Settings & operator=( Settings && ) noexcept = delete;

    virtual ~Settings() noexcept = default;

    sf::Vector2u get_window_size() const noexcept;
    double get_refresh_rate() const noexcept;
    bool get_vertical_sync() const noexcept;

  private:
    sf::Vector2u m_windowSize {};
    double m_refreshRate {};
    bool m_verticalSync {};
};
