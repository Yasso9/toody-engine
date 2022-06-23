#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

#include <string>
using namespace std::string_literals;
#include "main/window.hpp"

class View : public sf::View
{
  public:
    void set_zoom( float newZoom )
    {
        if ( newZoom == 0 )
        {
            /// @todo revoir les classe throw pendant les exceptions et vérifié si elles sont cohérentes dans tous le programme
            std::runtime_error { "Division by Zero" };
        }

        this->setSize( Window::get_instance().get_size().to_float() / newZoom );
    }

    math::Vector2F get_zoom() const
    {
        return Window::get_instance().get_size().to_float()
               / math::Vector2F { this->getSize() };
    }

    math::Vector2F get_size() const { return this->getSize(); }
    math::Vector2F get_center() const { return this->getCenter(); }

    /// @brief position where the view rectangle begin
    math::Vector2F get_position() const
    {
        return this->get_center() - ( this->get_size() / 2.f );
    }

  private:
    using sf::View::getCenter;
    using sf::View::getSize;
};