#pragma once

#include "graphics2D/sfml.hpp"

struct S_MovementKey
{
    sf::Keyboard::Key up;
    sf::Keyboard::Key right;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;

    S_MovementKey( sf::Keyboard::Key aUp, sf::Keyboard::Key aRight,
                   sf::Keyboard::Key aDown, sf::Keyboard::Key aLeft )
      : up( aUp ), right( aRight ), down( aDown ), left( aLeft )
    {}
};

S_MovementKey const MOVEMENT_ZQSD {
    sf::Keyboard::Z, sf::Keyboard::Q, sf::Keyboard::S, sf::Keyboard::D
};
S_MovementKey const MOVEMENT_ILKJ {
    sf::Keyboard::Z, sf::Keyboard::Q, sf::Keyboard::S, sf::Keyboard::D
};
S_MovementKey const MOVEMENT_5321 {
    sf::Keyboard::Z, sf::Keyboard::Q, sf::Keyboard::S, sf::Keyboard::D
};
S_MovementKey const MOVEMENT_ARROW {
    sf::Keyboard::Z, sf::Keyboard::Q, sf::Keyboard::S, sf::Keyboard::D
};