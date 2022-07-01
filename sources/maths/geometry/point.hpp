#pragma once

#include "maths/vector2.hpp"

namespace math
{
    template < C_Primitive Type >
    struct Segment;
    template < C_Primitive Type, unsigned int NB_OF_POINT >
    class Polygon;

    enum class E_Orientation
    {
        Collinear = 0,
        ClockWise,
        CounterClockWise,
        Unkown
    };

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

        template < unsigned int NB_OF_POINT >
        bool is_inside( Polygon< Type, NB_OF_POINT > polygon ) const;
    };

    template < C_Primitive Type >
    E_Orientation get_orientation( Vector2< Type > pointA,
                                   Vector2< Type > pointB,
                                   Vector2< Type > pointC );

    template < C_Primitive Type >
    bool are_collinear( Vector2< Type > pointA, Vector2< Type > pointB,
                        Vector2< Type > pointC );
} // namespace math

#include "point.tpp"