#pragma once

#include <vector>

#include "maths/geometry/point.hpp"
#include "maths/geometry/rectangle.hpp"
#include "tools/concepts.hpp"

namespace math
{
    template < C_Primitive Type >
    class Polygon
    {
        std::vector< Point< Type > > m_points;

      public:
        Polygon();
        Polygon( Rectangle< Type > rectangle );

        virtual ~Polygon() = default;

        Point< Type > const & operator[]( unsigned int index ) const;
        Point< Type > & operator[]( unsigned int index );
        unsigned int get_number_of_points() const;

        void set_rectangle( Rectangle< Type > rectangle );

        std::string print() const;

        std::vector< Segment< Type > > get_segments() const;
        std::vector< Point< Type > > get_points() const;
    };

    using PolygonF = Polygon< float >;
    using PolygonI = Polygon< int >;
    using PolygonU = Polygon< unsigned int >;

    template < C_Primitive Type >
    bool is_intersection( Polygon< Type > polygonLeft,
                          Polygon< Type > polygonRight );

} // namespace math

#include "polygon.tpp"
