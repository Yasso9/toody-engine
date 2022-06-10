#include "rectangle.hpp"

namespace math
{
    template < C_Primitive Type >
    Rectangle< Type >::Rectangle() : position( 0, 0 ), size( 0, 0 )
    {}

    template < C_Primitive Type >
    Rectangle< Type >::Rectangle( Vector2< Type > const & aPosition,
                                  Vector2< Type > const & aSize )
      : position( aPosition ), size( aSize )
    {}
    template < C_Primitive Type >
    Rectangle< Type >::Rectangle( Type const & x, Type const & y,
                                  Type const & width, Type const & height )
      : position( x, y ), size( width, height )
    {}
} // namespace math