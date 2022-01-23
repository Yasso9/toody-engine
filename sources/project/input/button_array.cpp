#include "button_array.hpp"

#include <algorithm>
#include <cassert>

ButtonArray::ButtonArray( sf::Font const & font,
                          std::vector<std::string> const & buttonsString )
  : m_font( font ), m_direction( ButtonArray::Direction::Horizontal )
{
    assert( ! buttonsString.empty()
            && "There must be at least one button on the "
               "ButtonArray constructor class" );

    sf::Vector2f localPosition { 0.f, 0.f };

    for ( std::size_t i_buttonNumber { 0 };
          i_buttonNumber < buttonsString.size();
          ++i_buttonNumber )
    {
        this->m_buttons.push_back(
            Button { this->m_font, buttonsString[i_buttonNumber] } );

        Button & button { this->m_buttons[i_buttonNumber] };

        button.set_size( 30.f );
        button.setPosition( this->getPosition() + localPosition );

        if ( this->m_direction == ButtonArray::Direction::Horizontal )
        {
            localPosition.x += button.get_size().x;
        }
        else if ( this->m_direction == ButtonArray::Direction::Vertical )
        {
            localPosition.y += button.get_size().y;
        }
        else
        {
            assert( false && "The direction must be vertical or horizontal" );
        }
    }
}

sf::Vector2f ButtonArray::get_size() const noexcept
{
    sf::Vector2f buttonSize { 0.f, 0.f };

    for ( auto const & button : this->m_buttons )
    {
        if ( this->m_direction == ButtonArray::Direction::Horizontal )
        {
            buttonSize.x += button.get_size().x;

            buttonSize.y = std::max( buttonSize.y, button.get_size().y );
        }
        else if ( this->m_direction == ButtonArray::Direction::Vertical )
        {
            buttonSize.y += button.get_size().y;

            buttonSize.x = std::max( buttonSize.x, button.get_size().x );
        }
        else
        {
            assert( false && "The direction must be vertical or horizontal" );
        }
    }

    return buttonSize * this->getScale();
}

void ButtonArray::set_size( sf::Vector2f const & size ) noexcept
{
    this->setScale( size / this->get_size() );
}

void ButtonArray::set_size( float const & sizeX, float const & sizeY ) noexcept
{
    this->set_size( sf::Vector2f { sizeX, sizeY } );
}

int ButtonArray::update( sf::Vector2f const & position, bool const & click )
{
    for ( std::size_t i_buttonNummber { 0 };
          i_buttonNummber < this->m_buttons.size();
          ++i_buttonNummber )
    {
        bool const isPressed {
            this->m_buttons.at( i_buttonNummber ).update( position, click )
        };

        if ( isPressed == true )
        {
            return static_cast<int>( i_buttonNummber );
        }
    }

    return -1;
}

void ButtonArray::draw( sf::RenderTarget & target,
                        sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    for ( auto const & button : this->m_buttons )
    {
        target.draw( button, states );
    }
}