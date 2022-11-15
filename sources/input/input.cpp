#include "input.hpp"

#include "main/window.hpp"           // for Window
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/vector2.tpp"         // for Vector2::operator=, Vector2::ope...
#include "tools/singleton.tpp"       // for Singleton::get_instance

namespace input
{
    static bool handle_input ()
    {
        return Window::get_instance().has_absolute_focus();
    }

    bool is_pressed ( sf::Keyboard::Key key )
    {
        if ( ! handle_input() )
            return false;

        return sf::Keyboard::isKeyPressed( key );
    }

    bool is_pressed ( sf::Mouse::Button mouseButton )
    {
        if ( ! handle_input() )
            return false;

        return sf::Mouse::isButtonPressed( mouseButton );
    }

    math::Vector2I get_mouse_position ()
    {
        bool s_programBeginWithoutInpuHandled { true };

        if ( ! handle_input() && s_programBeginWithoutInpuHandled )
            return MOUSE_DEFAULT_POSITION;

        s_programBeginWithoutInpuHandled = false;

        math::Vector2I const currentPosition {
            sf::Mouse::getPosition( Window::get_instance() ) };
        static math::Vector2I s_lastPosition { currentPosition };

        if ( ! handle_input() )
            return s_lastPosition;

        s_lastPosition = currentPosition;

        return currentPosition;
    }

    math::Vector2F get_mouse_position_relative ( View view )
    {
        return ( input::get_mouse_position().to_float() / view.get_zoom() )
               + view.get_position();
    }

    static math::Vector2F g_mouseMovement { 0.f, 0.f };

    void reset_mouse_movement ()
    {
        g_mouseMovement = { 0.f, 0.f };
    }

    void set_mouse_movement ( math::Vector2F mouseMovement )
    {
        math::Vector2F const  currentMousePosition { mouseMovement };
        static math::Vector2F s_lastMousePosition { currentMousePosition };

        g_mouseMovement = { s_lastMousePosition - currentMousePosition };

        s_lastMousePosition = currentMousePosition;
    }

    math::Vector2F get_mouse_movement ()
    {
        if ( ! handle_input() )
            return { 0.f, 0.f };

        return g_mouseMovement;
    }

    namespace
    {
        float g_mouseScroll { 0.f };
    }  // namespace

    float get_mouse_scroll ()
    {
        return g_mouseScroll;
    }

    void set_mouse_scroll ( float mouseScrollDelta )
    {
        g_mouseScroll = mouseScrollDelta;
    }

    void set_mouse_position ( math::PointI newMousePosition )
    {
        if ( ! handle_input() )
            return;

        sf::Mouse::setPosition( newMousePosition, Window::get_instance() );
    }

    math::Vector2F get_movement_vector (
        S_KeyboardMove movementKey, bool invertMovement )
    {
        if ( ! handle_input() )
            return { 0.f, 0.f };

        math::Vector2F moveDirection { 0.f, 0.f };
        if ( input::is_pressed( movementKey.up ) )
        {
            moveDirection += math::Vector2F { 0.f, -1.f };
        }
        if ( input::is_pressed( movementKey.right ) )
        {
            moveDirection += math::Vector2F { 1.f, 0.f };
        }
        if ( input::is_pressed( movementKey.down ) )
        {
            moveDirection += math::Vector2F { 0.f, 1.f };
        }
        if ( input::is_pressed( movementKey.left ) )
        {
            moveDirection += math::Vector2F { -1.f, 0.f };
        }

        if ( invertMovement )
        {
            moveDirection = -moveDirection;
        }

        return moveDirection;
    }

};  // namespace input