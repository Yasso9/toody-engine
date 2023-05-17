#pragma once

#include "maths/geometry/point.hpp"

namespace math
{
    template< cPrimitive Type >
    class Line
    {
      public:
        Point< Type > pointA;
        Point< Type > pointB;

        Line() noexcept : Line( { 0, 0 }, { 0, 0 } ) {}

        Line( Point< Type > point1, Point< Type > point2 )
          : pointA( point1 ), pointB( point2 )
        {}

        bool is_intersected_by ( Line< Type > segment ) const;

        template< typename TypeBis >
        friend std::ostream & operator<< ( std::ostream &          stream,
                                           Line< TypeBis > const & line );
    };

    using LineF = Line< float >;
    using LineI = Line< int >;
    using LineU = Line< unsigned int >;

    template< cPrimitive Type >
    bool is_intersection ( Line< Type > segment1, Line< Type > segment2 );
}  // namespace math

#include "line.tpp"