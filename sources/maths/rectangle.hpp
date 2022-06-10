#pragma once

#include "tools/concepts.hpp"

#include "vector2.hpp"

namespace math
{
    template < C_Primitive Type >
    class Rectangle
    {
      public:
        Vector2< Type > position;
        Vector2< Type > size;

        Rectangle();
        Rectangle( Vector2< Type > const & aPosition,
                   Vector2< Type > const & aSize );
        Rectangle( Type const & x, Type const & y, Type const & width,
                   Type const & height );
        virtual ~Rectangle() = default;
    };

    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;
} // namespace math

#include "rectangle.tpp"