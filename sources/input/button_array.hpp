#pragma once

#include "input/button.hpp"

class ButtonArray final : public TransformableComponent2D
{
  public:
    enum class E_Direction
    {
        Horizontal = 0,
        Vertical
    };

    ButtonArray( sf::Font const & font = sf::Font {} );
    virtual ~ButtonArray() = default;

    sf::Vector2f get_size() const;

    void set_size( sf::Vector2f const & size );
    void set_size( float const & sizeX, float const & sizeY );
    void set_font( sf::Font const & font );
    void set_strings( std::vector< std::string > const & buttonStrings );
    void set_direction( ButtonArray::E_Direction const & buttonsDirection );

    int update_button_array( sf::Vector2f const & position,
                             bool const & click );

  private:
    void render_before( Render & render ) const override;

    std::vector< Button > m_buttons;
    std::vector< std::string > m_strings;
    sf::Font m_font;
    ButtonArray::E_Direction m_direction;
    unsigned int m_characterSize;
};