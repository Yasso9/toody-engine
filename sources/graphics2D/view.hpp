#pragma once

#include <SFML/Graphics/View.hpp>  // for View

#include "maths/geometry/point.hpp"
#include "maths/geometry/rectangle.hpp"
#include "maths/vector2.hpp"  // for Vector2F

class View : public sf::View
{
  public:
    View() : sf::View {} {}

    View( sf::View view ) noexcept : sf::View { view } {}

    math::Vector2F get_zoom () const;
    void           set_zoom ( float newZoom );
    void           zoom ( float factor );

    /// @brief Size of the view
    math::Vector2F   get_size () const;
    /// @brief Position of the center of the view
    math::Vector2F   get_center () const;
    /// @brief Position where the view rectangle begin
    math::PointF     get_position () const;
    /// @brief Position and size of the view (e.g. Rectangle)
    math::RectangleF get_rectangle () const;

    /// @brief Check if a point is contained in the view
    bool contain ( math::PointF point ) const;

  private:
    using sf::View::getCenter;
    using sf::View::getSize;
    using sf::View::zoom;
};