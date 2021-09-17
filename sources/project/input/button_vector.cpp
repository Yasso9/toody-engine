#include "button_vector.hpp"

// Changer unsigned int => size_t ?

ButtonVector::ButtonVector( sf::Font const & font,
    std::vector<std::string> const & buttonsString )
{
    assert(
        !buttonsString.empty()
        && "There must be at least one button on the ButtonVector constructor class"
    );

    for ( unsigned int key { 0u }; key < buttonsString.size(); ++key )
    {
        this->m_buttonArray.insert( {
            key,
            Button { font, buttonsString[key] }
        } );

        this->m_buttonArray.at( key ).set_position(
            static_cast<float>(key) * 200.f + 400.f, 400.f
        );
    }
}

int ButtonVector::update_press( sf::Vector2f const & position )
{
    // There's a click, make of all the button to not pressed
    this->reset_press();
    // And search if any of the button is pressed
    for ( auto & button : this->m_buttonArray )
    {
        if ( button.second.is_inside( position ) )
        {
            button.second.set_pressed( true );

            return static_cast<int>(button.first);
        }
    }

    // No button is pressed
    return -1;
}

int ButtonVector::update_press( sf::Vector2f const & position, int const & buttonNumber )
{
    int const buttonNumberPressed { this->update_press( position ) };

    if ( buttonNumberPressed == -1 )
    {
        return buttonNumber;
    }

    return buttonNumberPressed;
}

void ButtonVector::update_select( sf::Vector2f const & position )
{
    this->reset_selection();

    for ( auto & button : this->m_buttonArray )
    {
        if ( button.second.is_inside( position ) )
        {
            button.second.set_selected( true );
        }
    }
}

void ButtonVector::render( sf::RenderWindow & target )
{
    for ( auto & button : this->m_buttonArray )
    {
        target.draw( button.second );
    }
}

void ButtonVector::reset_press()
{
    for ( auto & button : this->m_buttonArray )
    {
        button.second.set_pressed( false );
    }
}

void ButtonVector::reset_selection()
{
    for ( auto & button : this->m_buttonArray )
    {
        button.second.set_selected( false );
    }
}