#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "maths/geometry/point.hpp"
#include "tools/concepts.hpp"

namespace math
{
    template< cPrimitive Type >
    class Rectangle
    {
      public:
        Point< Type >   position;
        Vector2< Type > size;

        Rectangle() : Rectangle( 0.f, 0.f, 0.f, 0.f ) {}

        Rectangle( Point< Type > aPosition, Vector2< Type > aSize )
          : Rectangle( aPosition.x, aPosition.y, aSize.x, aSize.y )
        {}

        Rectangle( Type x, Type y, Type width, Type height )
          : position( x, y ), size( width, height )
        {}

        math::Vector2< Type > end_position () const { return position + size; }

        bool contain ( math::Point< Type > point ) const
        {
            return point.is_inside( *this );
        }

        operator sf::Rect< Type > () const
        {
            return sf::Rect< Type > { position.x, position.y, size.x, size.y };
        }

        Rectangle< Type > operator+ ( Vector2< Type > const & rhs )
        {
            return Rectangle< Type > { position + rhs, size };
        }

        Rectangle< Type > operator- ( Vector2< Type > const & rhs )
        {
            return Rectangle< Type > { position - rhs, size };
        }

        template< typename TypeBis >
        friend std::ostream & operator<< (
            std::ostream & stream, Rectangle< TypeBis > const & rectangle )
        {
            return stream << "{ Position:" << rectangle.position
                          << " Size:" << rectangle.size << " }";
        }
    };

    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;
}  // namespace math