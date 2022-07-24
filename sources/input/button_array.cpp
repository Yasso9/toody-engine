#include "button_array.hpp"

#include <algorithm>

#include "tools/assertion.hpp"

ButtonArray::ButtonArray( sf::Font const & font /* = sf::Font {} */ )
  : m_buttons(),
    m_strings(),
    m_font( font ),
    m_direction( ButtonArray::E_Direction::Horizontal ),
    m_characterSize( 30.f )
{}

sf::Vector2f ButtonArray::get_size() const
{
    sf::Vector2f buttonSize { 0.f, 0.f };

    for ( auto const & button : this->m_buttons )
    {
        if ( this->m_direction == ButtonArray::E_Direction::Horizontal )
        {
            buttonSize.x += button.get_size().x;

            buttonSize.y = std::max( buttonSize.y, button.get_size().y );
        }
        else if ( this->m_direction == ButtonArray::E_Direction::Vertical )
        {
            buttonSize.y += button.get_size().y;

            buttonSize.x = std::max( buttonSize.x, button.get_size().x );
        }
        else
        {
            ASSERTION( false, "The direction must be vertical or horizontal"s );
        }
    }

    return buttonSize * this->getScale();
}

void ButtonArray::set_size( sf::Vector2f const & size )
{
    this->setScale( size / this->get_size() );
}

void ButtonArray::set_size( float const & sizeX, float const & sizeY )
{
    this->set_size( sf::Vector2f { sizeX, sizeY } );
}

void ButtonArray::set_font( sf::Font const & font )
{
    this->m_font = font;
}

void ButtonArray::set_strings(
    std::vector< std::string > const & buttonStrings )
{
    this->m_strings = buttonStrings;

    sf::Vector2f localPosition { 0.f, 0.f };

    this->m_buttons.clear();

    for ( std::size_t i_button { 0 }; i_button < buttonStrings.size();
          ++i_button )
    {
        this->m_buttons.push_back( Button { this->m_font } );

        // To simplify writing
        Button & button { this->m_buttons[i_button] };

        button.set_string( buttonStrings[i_button] );
        button.set_size( this->m_characterSize );
        button.setPosition( this->getPosition() + localPosition );

        if ( this->m_direction == ButtonArray::E_Direction::Horizontal )
        {
            localPosition.x += button.get_size().x;
        }
        else if ( this->m_direction == ButtonArray::E_Direction::Vertical )
        {
            localPosition.y += button.get_size().y;
        }
        else
        {
            ASSERTION( false, "The direction must be vertical or horizontal"s );
        }
    }
}

void ButtonArray::set_direction(
    ButtonArray::E_Direction const & buttonsDirection )
{
    this->m_direction = buttonsDirection;

    // Update of the button array
    this->set_strings( this->m_strings );
}

int ButtonArray::update_button_array( sf::Vector2f const & position,
                                      bool const & click )
{
    for ( std::size_t i_buttonNummber { 0 };
          i_buttonNummber < this->m_buttons.size();
          ++i_buttonNummber )
    {
        bool const isPressed {
            this->m_buttons[i_buttonNummber].update_button( position, click )
        };

        if ( isPressed )
        {
            return static_cast< int >( i_buttonNummber );
        }
    }

    return -1;
}

void ButtonArray::render_before( Render & render ) const
{
    for ( Button const & button : this->m_buttons )
    {
        button.render_all( render );
    }
}