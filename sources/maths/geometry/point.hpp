#pragma once

#include "maths/vector2.hpp"

namespace math
{
    enum class E_Orientation
    {
        Collinear = 0,
        ClockWise,
        CounterClockWise,
        Unkown
    };

    template < C_Primitive Type >
    class Segment;
    template < C_Primitive Type >
    class Rectangle;
    template < C_Primitive Type >
    class Polygon;

    template < C_Primitive Type >
    class Point : public Vector2< Type >
    {
      public:
        Point() noexcept : Vector2< Type >() {}
        Point( Type xAxisValue, Type yAxisValue )
          : Vector2< Type >( xAxisValue, yAxisValue )
        {}

        /// @brief Copy Constructor
        Point( Vector2< Type > const & vector2D );

        explicit operator Vector2< Type >() const;
        template < C_Primitive OtherType >
            requires( not std::is_same_v< Type, OtherType > )
        explicit operator Point< OtherType >() const;

        Vector2< float > to_float() const        = delete;
        Vector2< unsigned int > to_u_int() const = delete;
        Vector2< int > to_int() const            = delete;
        Vector2< std::size_t > to_size_t() const = delete;

        Point< float > to_float() const
            requires( not std::is_same_v< Type, float > );
        Point< unsigned int > to_u_int() const
            requires( not std::is_same_v< Type, unsigned int > );
        Point< int > to_int() const requires( not std::is_same_v< Type, int > );
        Point< std::size_t > to_size_t() const
            requires( not std::is_same_v< Type, std::size_t > );

        bool is_inside( Segment< Type > segment ) const;
        bool is_inside( Point< Type > position, Vector2< Type > size ) const;
        bool is_inside( Rectangle< Type > rectangle ) const;
        bool is_inside( Polygon< Type > polygon ) const;
    };

    using PointF = Point< float >;
    using PointI = Point< int >;
    using PointU = Point< unsigned int >;

    template < C_Primitive Type >
    E_Orientation get_orientation( Point< Type > pointA, Point< Type > pointB,
                                   Point< Type > pointC );

    template < C_Primitive Type >
    bool are_collinear( Point< Type > pointA, Point< Type > pointB,
                        Point< Type > pointC );
} // namespace math

#include "point.tpp"