#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

namespace keyboard_move
{
    struct S_Key
    {
        // Clockwise directions input
        sf::Keyboard::Key up;
        sf::Keyboard::Key right;
        sf::Keyboard::Key down;
        sf::Keyboard::Key left;

        constexpr S_Key( sf::Keyboard::Key aUp, sf::Keyboard::Key aRight,
                         sf::Keyboard::Key aDown, sf::Keyboard::Key aLeft )
          : up( aUp ), right( aRight ), down( aDown ), left( aLeft )
        {}
    };
    constexpr S_Key ZDSQ {
        sf::Keyboard::Z, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::Q
    };
    constexpr S_Key ILKJ {
        sf::Keyboard::I, sf::Keyboard::L, sf::Keyboard::K, sf::Keyboard::J
    };
    constexpr S_Key NUMPAD5321 { sf::Keyboard::Numpad5,
                                 sf::Keyboard::Numpad3,
                                 sf::Keyboard::Numpad2,
                                 sf::Keyboard::Numpad1 };
    constexpr S_Key ARROW { sf::Keyboard::Up,
                            sf::Keyboard::Right,
                            sf::Keyboard::Down,
                            sf::Keyboard::Left };

    math::Vector2F get_vector( S_Key movementKey, bool invertMovement = false );

} // namespace keyboard_move
