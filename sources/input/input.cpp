#include "input.hpp"

#include "main/window.hpp"           // for Window
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/vector2.tpp"         // for Vector2::operator=, Vector2::ope...
#include "tools/singleton.tpp"       // for Singleton::get_instance

namespace input
{
    bool is_pressed ( Window const & window, sf::Keyboard::Key key )
    {
        if ( ! window.has_absolute_focus() )
        {
            return false;
        }

        return sf::Keyboard::isKeyPressed( key );
    }

    bool is_pressed ( Window const & window, sf::Mouse::Button mouseButton )
    {
        if ( ! window.has_absolute_focus() )
        {
            return false;
        }

        return sf::Mouse::isButtonPressed( mouseButton );
    }

    math::Vector2I get_mouse_position ( Window const & window )
    {
        bool s_programBeginWithoutInpuHandled { true };

        if ( ! window.has_absolute_focus() && s_programBeginWithoutInpuHandled )
        {
            return MOUSE_DEFAULT_POSITION;
        }

        s_programBeginWithoutInpuHandled = false;

        math::Vector2I const currentPosition {
            sf::Mouse::getPosition( window ) };
        static math::Vector2I s_lastPosition { currentPosition };

        if ( ! window.has_absolute_focus() )
        {
            return s_lastPosition;
        }

        s_lastPosition = currentPosition;

        return currentPosition;
    }

    math::Vector2F get_mouse_position_relative ( Window const & window,
                                                 View           view )
    {
        return ( input::get_mouse_position( window ).to_float()
                 / view.get_zoom( window.get_size().to_float() ) )
               + view.get_position();
    }

    /// @todo mouse movement should take the fact that a we can have multiple
    /// window or multiple mouse.
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

    math::Vector2F get_mouse_movement ( Window const & window )
    {
        if ( ! window.has_absolute_focus() )
        {
            return { 0.f, 0.f };
        }

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

    void set_mouse_position ( Window const & window,
                              math::PointI   newMousePosition )
    {
        if ( ! window.has_absolute_focus() )
        {
            return;
        }

        sf::Mouse::setPosition( newMousePosition, window );
    }

    math::Vector2F get_movement_vector ( Window const & window,
                                         S_KeyboardMove movementKey,
                                         bool           invertMovement )
    {
        if ( ! window.has_absolute_focus() )
        {
            return { 0.f, 0.f };
        }

        math::Vector2F moveDirection { 0.f, 0.f };
        if ( input::is_pressed( window, movementKey.up ) )
        {
            moveDirection += math::Vector2F { 0.f, -1.f };
        }
        if ( input::is_pressed( window, movementKey.right ) )
        {
            moveDirection += math::Vector2F { 1.f, 0.f };
        }
        if ( input::is_pressed( window, movementKey.down ) )
        {
            moveDirection += math::Vector2F { 0.f, 1.f };
        }
        if ( input::is_pressed( window, movementKey.left ) )
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