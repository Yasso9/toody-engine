#include "button_array.hpp"

// Changer unsigned int => size_t ?

ButtonArray::ButtonArray( sf::Font const & font,
                          std::vector<std::string> const & buttonsString )
  : m_size( 100.f, 50.f ), m_position( 0.f, 0.f )
{
    assert( ! buttonsString.empty()
            && "There must be at least one button on the "
               "ButtonArray constructor class" );

    for ( unsigned int key { 0u }; key < buttonsString.size(); ++key )
    {
        this->m_buttons.insert( { key, Button { font, buttonsString[key] } } );

        this->m_buttons.at( key ).set_size( this->m_size );

        this->m_buttons.at( key ).set_position(
            static_cast<float>( key ) * this->m_size.x + this->m_position.x,
            this->m_position.y );
    }
}

// TYPO faire ces fonctions
void set_size( sf::Vector2f const & /* size */ ) {}
void set_position( sf::Vector2f const & /* position */ ) {}

int ButtonArray::update_press( sf::Vector2f const & position )
{
    // There's a click, make of all the button to not pressed
    this->reset_press();
    // And search if any of the button is pressed
    for ( auto & button : this->m_buttons )
    {
        if ( button.second.is_inside( position ) )
        {
            button.second.set_pressed( true );

            return static_cast<int>( button.first );
        }
    }

    // No button is pressed
    return -1;
}

int ButtonArray::update_press( sf::Vector2f const & position,
                               int const & buttonNumber )
{
    int const buttonNumberPressed { this->update_press( position ) };

    if ( buttonNumberPressed == -1 )
    {
        return buttonNumber;
    }

    return buttonNumberPressed;
}

void ButtonArray::update_select( sf::Vector2f const & position )
{
    this->reset_selection();

    for ( auto & button : this->m_buttons )
    {
        if ( button.second.is_inside( position ) )
        {
            button.second.set_selected( true );
        }
    }
}

void ButtonArray::render( sf::RenderWindow & target )
{
    for ( auto & button : this->m_buttons )
    {
        target.draw( button.second );
    }
}

void ButtonArray::reset_press()
{
    for ( auto & button : this->m_buttons )
    {
        button.second.set_pressed( false );
    }
}

void ButtonArray::reset_selection()
{
    for ( auto & button : this->m_buttons )
    {
        button.second.set_selected( false );
    }
}