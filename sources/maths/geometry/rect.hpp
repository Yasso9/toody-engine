#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "maths/geometry/point.hpp"
#include "tools/concepts.hpp"

// TODO rework this class and improve it
// TODO create a TPP file
namespace math
{
    // TODO rename to Rect
    template< cPrimitive Type >
    class Rectangle
    {
      public:
        Point< Type >   pos;
        Vector2< Type > size;

        Rectangle() : Rectangle { 0, 0, 0, 0 } {}

        Rectangle( Point< Type > aPosition, Vector2< Type > aSize )
          : Rectangle { aPosition.x, aPosition.y, aSize.x, aSize.y }
        {}

        Rectangle( Type x, Type y, Type width, Type height )
          : pos { x, y }, size { width, height }
        {}

        Rectangle( sf::Rect< Type > const & sfRect )
          : Rectangle { sfRect.left, sfRect.top, sfRect.width, sfRect.height }
        {}

        math::Vector2< Type > bound_pos () const { return pos + size; }

        bool contains ( math::Point< Type > point ) const
        {
            return point.is_inside( *this );
        }

        operator sf::Rect< Type > () const
        {
            return sf::Rect< Type > { pos.x, pos.y, size.x, size.y };
        }

        // ####################################################################
        // ######################### RECT op RECT #############################
        // ####################################################################

        template< cPrimitive T >
        friend Rectangle< T > operator+ ( Rectangle< T > const & lhs,
                                          Rectangle< T > const & rhs );
        template< cPrimitive T >
        friend Rectangle< T > operator- ( Rectangle< T > const & lhs,
                                          Rectangle< T > const & rhs );
        template< cPrimitive T >
        friend Rectangle< T > operator+= ( Rectangle< T > &       lhs,
                                           Rectangle< T > const & rhs );
        template< cPrimitive T >
        friend Rectangle< T > operator-= ( Rectangle< T > &       lhs,
                                           Rectangle< T > const & rhs );

        // ####################################################################
        // ########################## RECT op VEC #############################
        // ####################################################################

        template< cPrimitive T >
        friend Rectangle< T > operator+ ( Rectangle< T > const & rect,
                                          Vector2< T > const &   vec2 );
        template< cPrimitive T >
        friend Rectangle< T > operator- ( Rectangle< T > const & rect,
                                          Vector2< T > const &   vec2 );
        template< cPrimitive T >
        friend Rectangle< T > operator+= ( Rectangle< T > &     rect,
                                           Vector2< T > const & vec2 );
        template< cPrimitive T >
        friend Rectangle< T > operator-= ( Rectangle< T > &     rect,
                                           Vector2< T > const & vec2 );

        template< typename TypeBis >
        friend std::ostream & operator<< (
            std::ostream & stream, Rectangle< TypeBis > const & rectangle )
        {
            return stream << "{ Pos:" << rectangle.pos
                          << " Size:" << rectangle.size << " }";
        }
    };

    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;
}  // namespace math

#include "rect_impl.hpp"
