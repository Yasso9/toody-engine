#pragma once

#include "graphics2D/tile/cell.hpp"

namespace tile
{
    class Size : public Cell
    {
      public:
        Size();
        Size( math::Vector2U position, Type type );
        Size( unsigned int x, unsigned int y, Type type );

        unsigned int index () const;
        void         index ( unsigned int newValue );
        Cell         to_cell () const;
    };
}  // namespace tile

DEFINE_FORMATTER( tile::Size, "{}", value.to_cell() );
