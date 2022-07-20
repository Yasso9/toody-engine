#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

class View : public sf::View
{
  public:
    View() : sf::View {} {}
    View( sf::View view ) noexcept : sf::View { view } {}

    void set_zoom( float newZoom );

    math::Vector2F get_zoom() const;

    math::Vector2F get_size() const;
    math::Vector2F get_center() const;

    /// @brief position where the view rectangle begin
    math::Vector2F get_position() const;

  private:
    using sf::View::getCenter;
    using sf::View::getSize;
};