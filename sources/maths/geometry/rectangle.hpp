#pragma once

#include "tools/concepts.hpp"

#include "maths/geometry/segment.hpp"
#include "maths/vector2.hpp"

namespace math
{
    template < C_Primitive Type >
    class Rectangle
    {
      public:
        Point< Type > position;
        Vector2< Type > size;

        Rectangle() : Rectangle( 0.f, 0.f, 0.f, 0.f ) {}
        Rectangle( Vector2< Type > aPosition, Vector2< Type > aSize )
          : Rectangle( aPosition.x, aPosition.y, aSize.x, aSize.y )
        {}
        Rectangle( Type x, Type y, Type width, Type height )
          : position( x, y ), size( width, height )
        {}
    };

    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;
} // namespace math