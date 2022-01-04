#pragma once

#include <project/tools/sfml.hpp>

// Faire un template de message en mode sdtvector
class Message final : public sf::Drawable, public sf::Transformable
{
  public:
    Message( sf::Font const & font );

    void initialize_text( std::size_t const & size );

    void update( sf::Vector2f const position,
                 std::vector<std::string> const & message = {} );

    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;

  private:
    /** @brief array of all the text to print to the screen */
    std::vector<sf::Text> m_textArray {};
    sf::Font const m_font;
};

class Cursor final : public sf::Drawable, public sf::Transformable
{
  public:
    Cursor();

    sf::Vector2f get_size() const noexcept;
    void set_size( sf::Vector2f const & size ) noexcept;
    void set_size( float const & sizeX, float const & sizeY ) noexcept;

    void update( sf::FloatRect const & rectangle );
    void update( sf::Vector2f const & position, sf::Vector2f const & size );

    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;

  private:
    sf::RectangleShape m_shape {};
};