#pragma once

#include <SFML/Window/Keyboard.hpp>  // for Keyboard, Keyboard::Key, Keyboar...

struct KeyboardMove
{
    // Clockwise directions input
    sf::Keyboard::Key up;
    sf::Keyboard::Key right;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;

    constexpr KeyboardMove( sf::Keyboard::Key aUp, sf::Keyboard::Key aRight,
                            sf::Keyboard::Key aDown, sf::Keyboard::Key aLeft )
      : up { aUp }, right { aRight }, down { aDown }, left { aLeft }
    {}
};

namespace input
{
    constexpr KeyboardMove ZDSQ { sf::Keyboard::Z, sf::Keyboard::D,
                                  sf::Keyboard::S, sf::Keyboard::Q };
    constexpr KeyboardMove ILKJ { sf::Keyboard::I, sf::Keyboard::L,
                                  sf::Keyboard::K, sf::Keyboard::J };
    constexpr KeyboardMove NUMPAD5321 {
        sf::Keyboard::Numpad5, sf::Keyboard::Numpad3, sf::Keyboard::Numpad2,
        sf::Keyboard::Numpad1 };
    constexpr KeyboardMove ARROW { sf::Keyboard::Up, sf::Keyboard::Right,
                                   sf::Keyboard::Down, sf::Keyboard::Left };
}  // namespace input
