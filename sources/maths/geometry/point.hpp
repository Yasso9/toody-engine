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
    template < C_Primitive Type, unsigned int NbOfPoints >
    class Polygon;

    template < C_Primitive Type >
    class Point : public Vector2< Type >
    {
      public:
        Point() : Vector2< Type >() {}
        Point( Type xAxisValue, Type yAxisValue )
          : Vector2< Type >( xAxisValue, yAxisValue )
        {}

        /// @brief Copy Constructor
        Point( Vector2< Type > const & vector2D );
        /// @brief Move Constructor
        Point( Vector2< Type > && vector2D ) noexcept;
        /// @brief Copy Assignement
        Point< Type > & operator=( Vector2< Type > const & vector2D );
        /// @brief Move Assignement
        Point< Type > & operator=( Vector2< Type > && vector2D ) noexcept;

        explicit operator Vector2< Type >() const;

        bool is_inside( Segment< Type > segment ) const;
        bool is_inside( Point< Type > position, Vector2< Type > size ) const;
        bool is_inside( Rectangle< Type > rectangle ) const;
        template < unsigned int NbOfPoints >
        bool is_inside( Polygon< Type, NbOfPoints > polygon ) const;
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