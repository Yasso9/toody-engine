#pragma once

#include "vector2.hpp"

namespace math
{
    /// @todo en faire un template
    class Rectangle
    {
      public:
        Vector2< float > position { 0.f, 0.f };
        Vector2< float > size { 0.f, 0.f };

        Rectangle() = default;
        Rectangle( Vector2< float > const & aPosition,
                   Vector2< float > const & aSize );
        Rectangle( float const & x, float const & y, float const & width,
                   float const & height );
        virtual ~Rectangle() = default;
    };
} // namespace math