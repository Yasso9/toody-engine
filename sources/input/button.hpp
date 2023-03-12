#pragma once

#include <string>  // for allocator, string

#include <SFML/Graphics/RectangleShape.hpp>  // for RectangleShape
#include <SFML/Graphics/Text.hpp>            // for Text
#include <SFML/System/Vector2.hpp>           // for Vector2f

#include "component/transformable2D.hpp"  // for TransformableComponent

class Render;

namespace sf
{
    class Font;
}  // namespace sf

class Button final : public Transformable2D
{
  public:
    Button( sf::Font const & font, std::string const & string = "Button" );
    virtual ~Button() = default;

    sf::Vector2f get_size () const;
    // void set_size( sf::Vector2f const & size );
    // void set_size( float const & sizeX, float const & sizeY );
    void         set_size ( unsigned int const & characterSize );

    void set_string ( std::string const & string );

    /**
     * @brief Update the button
     * @param position Position of the mouse
     * @param click true if there is a click from the mouse, false otherwise
     * @return true if there's a click in the button, false otherwise
     */
    bool update_button ( sf::Vector2f const & position, bool const & click );

  private:
    void set_selected ( bool const & isSelected );
    void set_pressed ( bool const & isPressed );

    /**
     * @brief Check if the position is inside the button or outside
     * @param position Position to check
     * @return true of the position is inside the button, false otherwise
     */
    bool is_inside ( sf::Vector2f const & position ) const;

    void synchronize_string ();

    void render_before ( RenderContext & context ) const override;

    /// @brief shape of the button
    sf::RectangleShape m_shape {};
    /// @brief text held inside the button
    sf::Text           m_text {};
};