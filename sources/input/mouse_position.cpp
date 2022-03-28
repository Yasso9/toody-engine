#include "mouse_position.hpp"

void MousePosition::set_overall( sf::Vector2f const & position )
{
    this->m_overall = position;
}

void MousePosition::set_press( sf::Vector2f const & position )
{
    this->m_press   = position;
    this->m_overall = position;
}

void MousePosition::set_release( sf::Vector2f const & position )
{
    this->m_release = position;
    this->m_overall = position;
}

sf::Vector2f MousePosition::get_overall() const
{
    return this->m_overall;
}

sf::Vector2f MousePosition::get_press() const
{
    return this->m_press;
}

sf::Vector2f MousePosition::get_release() const
{
    return this->m_release;
}
