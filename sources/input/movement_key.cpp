#include "movement_key.hpp"

namespace keyboard_move
{
    math::Vector2F get_vector( S_Key movementKey, bool invertMovement )
    {
        math::Vector2F moveDirection { 0.f, 0.f };
        if ( sf::Keyboard::isKeyPressed( movementKey.up ) )
        {
            moveDirection += math::Vector2F { 0.f, -1.f };
        }
        if ( sf::Keyboard::isKeyPressed( movementKey.right ) )
        {
            moveDirection += math::Vector2F { 1.f, 0.f };
        }
        if ( sf::Keyboard::isKeyPressed( movementKey.down ) )
        {
            moveDirection += math::Vector2F { 0.f, 1.f };
        }
        if ( sf::Keyboard::isKeyPressed( movementKey.left ) )
        {
            moveDirection += math::Vector2F { -1.f, 0.f };
        }

        if ( invertMovement )
        {
            moveDirection = -moveDirection;
        }

        return moveDirection;
    }
} // namespace keyboard_move