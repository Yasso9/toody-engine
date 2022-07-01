#pragma once

#include "tools/concepts.hpp"

#include "maths/point.hpp"
#include "maths/rectangle.hpp"

namespace math
{
    template < C_Primitive Type >
    class Quadrangle
    {
      public:
        Point< Type > topLeftPosition; // A
        Point< Type > topRightPosition; // B
        Point< Type > bottomRightPosition; // C
        Point< Type > bottomLeftPosition; // D

        unsigned int const NUMBER_OF_POINT { 4u };

        Quadrangle();
        Quadrangle( Point< Type > a, Point< Type > b, Point< Type > c,
                    Point< Type > d );
        Quadrangle( Rectangle< Type > rectangle );

        virtual ~Quadrangle() = default;

        Point< Type > operator[]( unsigned int index ) const;
        unsigned int get_number_of_point() const;

        /// @todo mettre un argument pour la rotation
        void set_rectangle( Rectangle< Type > rectangle );

        /// @todo avoir un std array a la place
        std::vector< Segment< Type > > get_segments() const;
        std::vector< Point< Type > > get_points() const;
    };

    using QuadrangleF = Quadrangle< float >;
    using QuadrangleI = Quadrangle< int >;
    using QuadrangleU = Quadrangle< unsigned int >;

    template < C_Primitive Type >
    bool is_intersection( Quadrangle< Type > quadrangleA,
                          Quadrangle< Type > quadrangleB );

} // namespace math

#include "quadrangle.tpp"
