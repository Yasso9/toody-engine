#pragma once

#include <type_traits>

#include "graphics2D/sfml.hpp"
#include "graphics3D/openGL.hpp"
#include "maths/maths.hpp"
#include "tools/singleton.hpp"

/// @brief check if the class can be drawn
template < typename Type >
concept Drawable = std::is_base_of< sf::Drawable, Type >::value;

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

    void gl_draw_elements( unsigned int const & vertexArrayObject,
                           GLenum const & primitiveType,
                           GLenum const & dataType,
                           std::size_t const & elementsSize ) const;
    void gl_draw_arrays( unsigned int const & vertexArrayObject,
                         GLenum const & primitiveType,
                         unsigned int const & arraySize ) const;

    template < Drawable DrawableClass >
    void sf_draw( DrawableClass const & target )
    {
        this->draw( target );
    }

  private:
    Window();

    using sf::RenderWindow::draw;

    void creation();
    void initialize();
};