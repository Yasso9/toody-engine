#include "button_array.hpp"

#include <cassert>

ButtonArray::ButtonArray( sf::Font const & font,
                          std::vector<std::string> const & buttonsString )
  : m_font( font ), m_direction()
{
    assert( ! buttonsString.empty()
            && "There must be at least one button on the "
               "ButtonArray constructor class" );

    for ( unsigned int i_buttonNummber { 0u };
          i_buttonNummber < buttonsString.size();
          ++i_buttonNummber )
    {
        this->m_buttons.insert(
            std::make_pair( i_buttonNummber, Button { this->m_font } ) );

        this->m_buttons.at( i_buttonNummber )
            .set_string( buttonsString[i_buttonNummber] );
        this->m_buttons.at( i_buttonNummber ).set_size( 100.f, 50.f );
        this->m_buttons.at( i_buttonNummber )
            .setPosition(
                static_cast<float>( i_buttonNummber )
                        * this->m_buttons.at( i_buttonNummber ).get_size().x
                    + this->getPosition().x,
                this->getPosition().y );
    }
}

sf::Vector2f ButtonArray::get_size() const noexcept
{
    sf::Vector2f buttonSize { 0.f, 0.f };

    for ( auto const & button : this->m_buttons )
    {
        if ( this->m_direction == ButtonArray::Direction::Horizontal )
        {
            buttonSize.x += button.second.get_size().x;

            if ( buttonSize.y < button.second.get_size().y )
            {
                buttonSize.y = button.second.get_size().y;
            }
        }
        else if ( this->m_direction == ButtonArray::Direction::Vertical )
        {
            buttonSize.y += button.second.get_size().y;

            if ( buttonSize.x < button.second.get_size().x )
            {
                buttonSize.x = button.second.get_size().x;
            }
        }
        else
        {
            // TYPO lancer une exeption ici
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
    for ( unsigned int i_buttonNummber { 0u };
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

    for ( auto & button : this->m_buttons )
    {
        target.draw( button.second, states );
    }
}