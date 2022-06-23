#include "view.hpp"

#include <string>
using namespace std::string_literals;
#include "main/window.hpp"

void View::set_zoom( float newZoom )
{
    if ( newZoom == 0 )
    {
        /// @todo revoir les classe throw pendant les exceptions et vérifié si elles sont cohérentes dans tous le programme
        std::runtime_error { "Division by Zero" };
    }

    this->setSize( Window::get_instance().get_size().to_float() / newZoom );
}

math::Vector2F View::get_zoom() const
{
    return Window::get_instance().get_size().to_float()
           / math::Vector2F { this->getSize() };
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