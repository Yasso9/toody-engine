#pragma once

#include <project/input/button.hpp>

class ButtonArray final : public sf::Drawable, public sf::Transformable
{
  public:
    enum class Direction
    {
        Horizontal = 0,
        Vertical
    };

    ButtonArray( sf::Font const & font,
                 std::vector<std::string> const & buttonsString = {
                     "Button" } );

    ButtonArray( const ButtonArray & ) noexcept = delete;
    ButtonArray( ButtonArray && ) noexcept = delete;
    ButtonArray & operator=( const ButtonArray & ) = delete;
    ButtonArray & operator=( ButtonArray && ) noexcept = delete;

    virtual ~ButtonArray() noexcept = default;

    sf::Vector2f get_size() const noexcept;
    void set_size( sf::Vector2f const & size ) noexcept;
    void set_size( float const & sizeX, float const & sizeY ) noexcept;

    int update( sf::Vector2f const & position, bool const & click );

  private:
    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;

    std::vector<Button> m_buttons {};
    sf::Font const m_font;
    Direction m_direction;
};