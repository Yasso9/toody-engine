#pragma once

#include <istream>
#include <ostream>

#include "maths/vector2.hpp"
#include "tools/concepts.hpp"

namespace math
{
    enum class E_Orientation
    {
        Collinear = 0,
        ClockWise,
        CounterClockWise,
        Unkown
    };

    template< cPrimitive Type >
    class Line;
    template< cPrimitive Type >
    class Rectangle;
    template< cPrimitive Type >
    class Polygon;

    template< cPrimitive Type >
    class Point : public Vector2< Type >
    {
      public:
        Point() noexcept : Vector2< Type >() {}

        Point( Type xAxisValue, Type yAxisValue ) noexcept
          : Vector2< Type >( xAxisValue, yAxisValue )
        {}

        /// @brief Copy Constructor
        Point( Vector2< Type > const & vector2D ) noexcept;

        explicit operator Vector2< Type > () const;
        template< cPrimitive OtherType >
        requires ( not std::is_same_v< Type, OtherType > )
        explicit operator Point< OtherType > () const;

        Vector2< float >        to_float () const  = delete;
        Vector2< unsigned int > to_u_int () const  = delete;
        Vector2< int >          to_int () const    = delete;
        Vector2< std::size_t >  to_size_t () const = delete;

        Vector2< Type > to_vector () const;

        Point< float > to_float () const
            requires ( not std::is_same_v< Type, float > );
        Point< unsigned int > to_u_int () const
            requires ( not std::is_same_v< Type, unsigned int > );
        Point< int > to_int () const
            requires ( not std::is_same_v< Type, int > );
        Point< std::size_t > to_size_t () const
            requires ( not std::is_same_v< Type, std::size_t > );

        bool is_inside ( Line< Type > segment ) const;
        bool is_inside ( Point< Type > position, Vector2< Type > size ) const;
        bool is_inside ( Rectangle< Type > rectangle ) const;
        bool is_inside ( Polygon< Type > polygon ) const;
    };

    using PointF = Point< float >;
    using PointI = Point< int >;
    using PointU = Point< unsigned int >;

    template< cPrimitive Type >
    E_Orientation get_orientation ( Point< Type > pointA, Point< Type > pointB,
                                    Point< Type > pointC );

    template< cPrimitive Type >
    bool are_collinear ( Point< Type > pointA, Point< Type > pointB,
                         Point< Type > pointC );
}  // namespace math

DEFINE_FORMATTER_TEMPLATE( math::Point, "{}", value.to_vector() );

#include "point.tpp"
