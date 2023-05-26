#pragma once

#include <SFML/Graphics/View.hpp>  // for View

#include "interface/window.hpp"          // for Window
#include "libraries/debug_window.hpp"    // for DebugWindow
#include "maths/geometry/point.hpp"      // for PointF
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F

class View : public DebugWindow
{
  public:
    sf::View m_sfView;

  private:
    float m_zoomMax;
    float m_zoomMin;

  public:
    View();
    View( sf::View const & sfView ) noexcept;
    // View( math::RectangleF rectangle );

    operator sf::View & ();
    operator sf::View const & () const;

    math::Vector2F get_zoom ( math::Vector2F windowSize ) const;
    math::Vector2F get_zoom ( Window const & window ) const;
    void           set_zoom ( float newZoom, math::Vector2F windowSize );
    void           set_zoom ( float newZoom, Window const & window );
    void           zoom ( float factor, math::Vector2F windowSize );
    void           zoom ( float factor, Window const & window );

    /// @brief Size of the view
    math::Vector2F get_size () const;
    /// @brief Position of the center of the view
    math::Vector2F get_center () const;
    /// @brief Position where the view rectangle begin (top left corner)
    math::PointF get_position () const;
    /// @brief Position and size of the view (e.g. Rectangle)
    math::RectangleF get_rectangle () const;

    float            get_rotation () const;
    math::RectangleF get_viewport () const;

    /// @brief Check if a point is contained in the view
    bool contain ( math::PointF point ) const;

    virtual void debug_window_content () override;
};
