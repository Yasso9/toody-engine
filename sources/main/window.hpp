#pragma once

#include <type_traits> // for is_base_of

#include <SFML/Graphics/RenderWindow.hpp> // for RenderWindow

#include "maths/vector2.hpp"   // for Vector2U
#include "tools/singleton.hpp" // for Singleton

namespace sf
{
    class Color;
    class Drawable;
} // namespace sf

/// @brief check if the class can be drawn
template < typename Type >
concept C_IsDrawable = std::is_base_of< sf::Drawable, Type >::value;

/// @todo maybe think to remove the singleton pattern for this class
class Window final : public sf::RenderWindow,
                     public Singleton< Window >
{
    // We must have the constuctor so the singleton can call it
    friend Window & Singleton< Window >::get_instance();

  public:
    virtual ~Window() = default;

    math::Vector2U get_size() const;

    float get_aspect_ratio() const;

    math::Vector2U get_center_position() const;

    bool is_hovered() const;
    /// @brief Check if the window have focus and if the mouse is inside the windows
    bool has_absolute_focus() const;

    void clear_all( sf::Color const & backgroundColor );

  private:
    Window();

    void creation();
    void initialize();
};