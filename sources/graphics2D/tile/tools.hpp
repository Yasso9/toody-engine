#pragma once

#include "maths/vector2.hpp"

namespace tile
{
    math::Vector2U pixel_to_tile_position ( math::Vector2U pixel_position );
    math::Vector2U tile_to_pixel_position ( math::Vector2U tile_position );
}  // namespace tile