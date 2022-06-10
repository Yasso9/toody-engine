#include "rectangle.hpp"

namespace math
{
    Rectangle::Rectangle( Vector2< float > const & aPosition,
                          Vector2< float > const & aSize )
      : position( aPosition ), size( aSize )
    {}
    Rectangle::Rectangle( float const & x, float const & y, float const & width,
                          float const & height )
      : position( x, y ), size( width, height )
    {}
} // namespace math