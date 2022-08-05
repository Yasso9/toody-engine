#pragma once

#include <SFML/Window/Keyboard.hpp>  // for Keyboard, Keyboard::Key, Keyboar...
#include <SFML/Window/Mouse.hpp>     // for Mouse, Mouse::Button

#include "maths/geometry/point.hpp"  // for PointI
#include "maths/vector2.hpp"         // for Vector2F, Vector2I

namespace input
{
    struct S_KeyboardMove
    {
        // Clockwise directions input
        sf::Keyboard::Key up;
        sf::Keyboard::Key right;
        sf::Keyboard::Key down;
        sf::Keyboard::Key left;

        constexpr S_KeyboardMove(
            sf::Keyboard::Key aUp, sf::Keyboard::Key aRight,
            sf::Keyboard::Key aDown, sf::Keyboard::Key aLeft )
          : up( aUp ), right( aRight ), down( aDown ), left( aLeft )
        {}
    };

    constexpr math::Vector2I MOUSE_DEFAULT_POSITION { 0, 0 };
    constexpr S_KeyboardMove ZDSQ {
        sf::Keyboard::Z, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::Q };
    constexpr S_KeyboardMove ILKJ {
        sf::Keyboard::I, sf::Keyboard::L, sf::Keyboard::K, sf::Keyboard::J };
    constexpr S_KeyboardMove NUMPAD5321 {
        sf::Keyboard::Numpad5, sf::Keyboard::Numpad3, sf::Keyboard::Numpad2,
        sf::Keyboard::Numpad1 };
    constexpr S_KeyboardMove ARROW {
        sf::Keyboard::Up, sf::Keyboard::Right, sf::Keyboard::Down,
        sf::Keyboard::Left };

    bool is_pressed ( sf::Keyboard::Key key );
    bool is_pressed ( sf::Mouse::Button mouseButton );

    math::Vector2I get_mouse_position ();
    math::PointI   get_mouse_position_point ();
    void           reset_mouse_movement ();
    void           set_mouse_position ( math::PointI newMousePosition );
    math::Vector2F get_mouse_movement ();
    void           set_mouse_movement ( math::Vector2F mouseMovement );
    float          get_mouse_scroll ();
    /// @remark must be used only by the pollevent MouseScroll event
    void           set_mouse_scroll ( float mouseScrollDelta );

    math::Vector2F get_movement_vector (
        S_KeyboardMove movementKey, bool invertMovement = false );
};  // namespace input