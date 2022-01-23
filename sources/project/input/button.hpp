#pragma once

#include <project/tools/sfml.hpp>

class Button final : public sf::Drawable, public sf::Transformable
{
  public:
    Button( sf::Font const & font, std::string const & string = "Button" );

    Button( const Button & ) noexcept = default;
    Button( Button && ) noexcept = default;
    Button & operator=( const Button & ) = delete;
    Button & operator=( Button && ) noexcept = delete;

    virtual ~Button() noexcept = default;

    sf::Vector2f get_size() const noexcept;
    // void set_size( sf::Vector2f const & size ) noexcept;
    // void set_size( float const & sizeX, float const & sizeY ) noexcept;
    void set_size( unsigned int const & characterSize ) noexcept;

    void set_string( std::string const & string );

    /**
     * @brief Update the button
     * @return true if there's a click in the button, false otherwise
     */
    bool update( sf::Vector2f const & position, bool const & click );

  private:
    void set_selected( bool const & isSelected ) noexcept;
    void set_pressed( bool const & isPressed ) noexcept;

    // TYPO mettre ça autre part, dans un geometry.cpp peut être
    /**
     * @brief Check if the position is inside the button or outside
     * @param position Position to check
     * @return true of the position is inside the button, false otherwise
     */
    bool is_inside( sf::Vector2f const & position ) const noexcept;

    void synchronize_string() noexcept;

    // TYPO Changer tout les draw pour qu'ils ressemblent à ça
    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;

    /// @brief shape of the button
    sf::RectangleShape m_shape {};
    /// @brief text held inside the button
    sf::Text m_text {};
};