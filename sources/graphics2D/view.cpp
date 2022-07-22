#include "view.hpp"

#include <string>
using namespace std::string_literals;
#include "input/input.hpp"
#include "main/window.hpp"

math::Vector2F View::get_zoom() const
{
    return Window::get_instance().get_size().to_float()
           / math::Vector2F { this->getSize() };
}
void View::set_zoom( float newZoom )
{
    if ( newZoom == 0 )
    {
        throw std::logic_error { "Division by Zero" };
    }

    this->setSize( Window::get_instance().get_size().to_float() / newZoom );
}

void View::zoom( float factor )
{
    sf::View::zoom( 1.f - factor );

    // Check if the zoom doesn't go too far
    constexpr float MAXIMUM_ZOOM { 15.f };
    constexpr float MINIMUM_ZOOM { 0.7f };
    if ( this->get_zoom().get_max() > MAXIMUM_ZOOM )
    {
        this->set_zoom( MAXIMUM_ZOOM );
    }
    if ( this->get_zoom().get_min() < MINIMUM_ZOOM )
    {
        this->set_zoom( MINIMUM_ZOOM );
    }
}

math::Vector2F View::get_size() const
{
    return this->getSize();
}
math::Vector2F View::get_center() const
{
    return this->getCenter();
}

/// @brief position where the view rectangle begin
math::Vector2F View::get_position() const
{
    return this->get_center() - ( this->get_size() / 2.f );
}