#pragma once

#include <type_traits>  // for is_base_of

#include <SFML/Graphics/RenderWindow.hpp>  // for RenderWindow

#include "maths/vector2.hpp"    // for Vector2U
#include "tools/singleton.hpp"  // for Singleton

namespace sf
{
    class Color;
    class Drawable;
}  // namespace sf

/// @brief check if the class can be drawn
template< typename T >
concept C_IsDrawable = std::is_base_of< sf::Drawable, T >::value;

class Window : public sf::RenderWindow
{
    friend class RenderContext;

  public:
    Window( std::string const & title );
    virtual ~Window();

    math::Vector2U get_size () const;
    float          get_aspect_ratio () const;
    math::Vector2U get_center_position () const;

    bool is_hovered () const;
    /// @brief Check if the window have focus and if the mouse is inside the
    /// windows
    bool has_absolute_focus () const;

    void reset_view ();

    void clear ( sf::Color const & backgroundColor = sf::Color( 0, 0, 0,
                                                                255 ) );

  private:
    using sf::RenderWindow::clear;
    using sf::RenderWindow::draw;
};