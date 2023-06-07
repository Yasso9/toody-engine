#pragma once

#include <optional>  // for optional

#include "graphics2D/tile/cellpos.hpp"   // for CellPos
#include "graphics2D/tile/position.hpp"  // for Position
#include "graphics2D/tile/size.hpp"      // for Size
#include "maths/vector2.hpp"             // for Vector2F

namespace tile
{
    // Abtract class for tilemap and tileset
    class Grid
    {
      public:
        Grid()          = default;
        virtual ~Grid() = default;

        virtual math::Vector2F get_position () const               = 0;
        virtual void           set_position ( math::Vector2F pos ) = 0;
        virtual tile::Size     get_size () const                   = 0;

        // position of the bottom right corner
        math::Vector2F get_bound_pos () const;

        std::optional< tile::CellPos > position (
            tile::Position const & pos ) const;
        std::optional< tile::CellPos > position ( unsigned int index ) const;

        bool is_compatible ( tile::CellPos const & position ) const;

        bool contains ( tile::Position const & position ) const;
    };
}  // namespace tile
