#pragma once

#include "segment.hpp"

namespace math
{
    template < C_Primitive Type >
    bool is_intersection( Segment< Type > segment1, Segment< Type > segment2 )
    {
        // Find the four orientations needed for general and
        // special cases
        E_Orientation s1a_s1b_s2a = get_orientation( segment1.pointA,
                                                     segment1.pointB,
                                                     segment2.pointA );

        E_Orientation o2 = get_orientation( segment1.pointA,
                                            segment1.pointB,
                                            segment2.pointB );

        E_Orientation o3 = get_orientation( segment2.pointA,
                                            segment2.pointB,
                                            segment1.pointA );

        E_Orientation o4 = get_orientation( segment2.pointA,
                                            segment2.pointB,
                                            segment1.pointB );

        // General case
        if ( s1a_s1b_s2a != o2 && o3 != o4 )
            return true;

        // Special Cases
        // p1, q1 and p2 are collinear and p2 lies on segment p1q1
        if ( s1a_s1b_s2a == E_Orientation::Collinear
             && segment2.pointA.is_inside( segment1 ) )
            return true;

        // p1, q1 and p2 are collinear and q2 lies on segment p1q1
        if ( o2 == E_Orientation::Collinear
             && segment2.pointB.is_inside( segment1 ) )
            return true;

        // p2, q2 and p1 are collinear and p1 lies on segment p2q2
        if ( o3 == E_Orientation::Collinear
             && segment1.pointA.is_inside( segment2 ) )
            return true;

        // p2, q2 and q1 are collinear and q1 lies on segment p2q2
        if ( o4 == E_Orientation::Collinear
             && segment1.pointB.is_inside( segment2 ) )
            return true;

        return false; // Doesn't fall in any of the above cases
    }
} // namespace math

#include "segment.tpp"
