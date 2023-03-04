#include "tools.hpp"

#include "graphics2D/constants.hpp"

namespace tile
{
    math::Vector2U pixel_to_tile_position ( math::Vector2U pixel_position )
    {
        return math::whole_part( pixel_position.to_float()
                                 / TILE_PIXEL_SIZE_VECTOR )
            .to_u_int();
    }

    math::Vector2U tile_to_pixel_position ( math::Vector2U tile_position )
    {
        return tile_position * TILE_PIXEL_SIZE_VECTOR;
    }
}  // namespace tile