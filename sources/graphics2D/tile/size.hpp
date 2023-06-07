#pragma once

#include "graphics2D/tile/position.hpp"  // for Cell

namespace tile
{
    class Size : public Position
    {
      public:
        Size();
        Size( math::Vector2U position, Type type );
        Size( unsigned int x, unsigned int y, Type type );

        unsigned int index () const;
        void         index ( unsigned int newValue );
        Position     to_cell () const;

        unsigned int nb_of_tile () const;
    };
}  // namespace tile

DEFINE_FORMATTER( tile::Size, "{}", value.to_cell() );
