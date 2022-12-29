#pragma once

#include "maths/geometry/point.hpp"

namespace math
{
    template< C_Primitive Type >
    class Segment
    {
      public:
        Point< Type > pointA;
        Point< Type > pointB;

        Segment() noexcept : Segment( { 0, 0 }, { 0, 0 } ) {}

        Segment( Point< Type > point1, Point< Type > point2 )
          : pointA( point1 ), pointB( point2 )
        {}

        bool is_intersected_by ( Segment< Type > segment ) const;
    };

    using SegmentF = Segment< float >;
    using SegmentI = Segment< int >;
    using SegmentU = Segment< unsigned int >;

    template< C_Primitive Type >
    bool is_intersection ( Segment< Type > segment1, Segment< Type > segment2 );
}  // namespace math

#include "segment.tpp"