#pragma once

#include <iosfwd>
#include <ostream>
#include <vector>

#include "maths/geometry/point.hpp"
#include "maths/geometry/rectangle.hpp"
#include "tools/concepts.hpp"
#include "tools/printable.hpp"

namespace math
{
    template< C_Primitive Type >
    class Polygon : public Printable
    {
        std::vector< Point< Type > > m_points;

      public:
        Polygon();
        Polygon( Rectangle< Type > rectangle );

        virtual ~Polygon() = default;

        Point< Type > const & operator[] ( unsigned int index ) const;
        Point< Type > &       operator[] ( unsigned int index );
        unsigned int          get_number_of_points () const;

        void set_rectangle ( Rectangle< Type > rectangle );

        /**
         * @brief Move all Points to the same distance
         * @param distance value of what all points should be moved
         */
        void move ( math::Vector2< Type > distance )
        {
            for ( auto & point : m_points )
            {
                point += distance;
            }
        }

        std::string print () const;

        std::vector< Segment< Type > > get_segments () const;
        std::vector< Point< Type > >   get_points () const;
        std::vector< Point< Type > > & get_points ();

        Polygon< Type > operator+ ( math::Vector2< Type > vector );

        void print_to_stream ( std::ostream & stream ) const override;
    };

    using PolygonF = Polygon< float >;
    using PolygonI = Polygon< int >;
    using PolygonU = Polygon< unsigned int >;

    template< C_Primitive Type >
    bool is_intersection (
        Polygon< Type > polygonLeft, Polygon< Type > polygonRight );

}  // namespace math

#include "polygon.tpp"
