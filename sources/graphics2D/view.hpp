#pragma once

#include <SFML/Graphics/View.hpp>  // for View

#include "maths/vector2.hpp"  // for Vector2F

class View : public sf::View
{
  public:
    View() : sf::View {} {}

    View( sf::View view ) noexcept : sf::View { view } {}

    math::Vector2F get_zoom () const;
    void           set_zoom ( float newZoom );

    void zoom ( float factor );

    math::Vector2F get_size () const;
    math::Vector2F get_center () const;

    /// @brief position where the view rectangle begin
    math::Vector2F get_position () const;

  private:
    using sf::View::getCenter;
    using sf::View::getSize;
    using sf::View::zoom;
};