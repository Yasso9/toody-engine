#pragma once

#include "graphics/sfml.hpp"

class MousePosition
{
  public:
    void set_overall( sf::Vector2f const & position );
    void set_press( sf::Vector2f const & position );
    void set_release( sf::Vector2f const & position );

    sf::Vector2f get_overall() const;
    sf::Vector2f get_press() const;
    sf::Vector2f get_release() const;

  private:
    sf::Vector2f m_overall;
    sf::Vector2f m_press;
    sf::Vector2f m_release;
};