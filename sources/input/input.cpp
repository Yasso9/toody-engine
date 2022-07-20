#include "input.hpp"

#include "main/window.hpp"

namespace input
{
    static bool handle_input()
    {
        return Window::get_instance().has_absolute_focus();
    }

    bool is_pressed( sf::Keyboard::Key key )
    {
        if ( ! handle_input() )
            return false;

        return sf::Keyboard::isKeyPressed( key );
    }
    bool is_pressed( sf::Mouse::Button mouseButton )
    {
        if ( ! handle_input() )
            return false;

        return sf::Mouse::isButtonPressed( mouseButton );
    }

    math::Vector2I get_mouse_position()
    {
        static math::Vector2I lastPosition { MOUSE_POSITION_DEFAULT };

        math::Vector2I const currentPosition { sf::Mouse::getPosition(
            Window::get_instance() ) };

        if ( ! handle_input() )
            return lastPosition;

        lastPosition = currentPosition;

        return currentPosition;
    }

    math::PointI get_mouse_position_point()
    {
        return math::PointI { get_mouse_position() };
    }

    void set_mouse_position( math::PointI newMousePosition )
    {
        if ( ! handle_input() )
            return;

        sf::Mouse::setPosition( newMousePosition, Window::get_instance() );
    }

}; // namespace input