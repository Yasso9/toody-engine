#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

namespace input
{
    constexpr math::Vector2I MOUSE_POSITION_DEFAULT { 0, 0 };

    bool is_pressed( sf::Keyboard::Key key );
    bool is_pressed( sf::Mouse::Button mouseButton );
    math::Vector2I get_mouse_position();
    math::PointI get_mouse_position_point();
    void set_mouse_position( math::PointI newMousePosition );
}; // namespace input