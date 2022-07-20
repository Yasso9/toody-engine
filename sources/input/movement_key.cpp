#include "movement_key.hpp"

#include "input/input.hpp"

namespace keyboard_move
{
    math::Vector2F get_vector( S_Key movementKey, bool invertMovement )
    {
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
} // namespace keyboard_move