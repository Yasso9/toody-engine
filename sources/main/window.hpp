#pragma once

#include "graphics/openGL.hpp"
#include "graphics/sfml.hpp"
#include "tools/singleton.hpp"

class Window final : public sf::RenderWindow,
                     public Singleton< Window >
{
    // We must have the constuctor so the singleton can call it
    friend Window & Singleton< Window >::get_instance();

  public:
    virtual ~Window() = default;

    sf::Vector2f get_size_f() const;
    sf::Vector2i get_size_i() const;

    float get_aspect_ratio() const;

    sf::Vector2u get_center_position_u() const;
    sf::Vector2f get_center_position_f() const;

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

    // TYPO use concept to verify that is drawable
    template < class DrawableClass >
    void sf_draw( DrawableClass const & target )
    {
        this->draw( target );
    }

  private:
    Window();

    void create();
    void initialize();
};