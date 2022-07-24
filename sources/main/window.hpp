#pragma once

#include <type_traits>

#include "graphics2D/sfml.hpp"
#include "graphics3D/openGL.hpp"
#include "maths/maths.hpp"
#include "tools/singleton.hpp"

class Component3D;

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