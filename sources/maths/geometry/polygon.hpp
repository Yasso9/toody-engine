#pragma once

#include <array>

#include "tools/concepts.hpp"

#include "maths/geometry/point.hpp"
#include "maths/geometry/rectangle.hpp"

namespace math
{
    template < unsigned int number >
    concept C_Quadrangle = ( number == 4u );

    /// @todo require that number of points isn't equal to 0
    template < C_Primitive Type, unsigned int NbOfPoints >
    class Polygon
    {
        std::array< Point< Type >, NbOfPoints > m_points;

      public:
        Polygon();
        Polygon( Rectangle< Type > rectangle ) requires(
            C_Quadrangle< NbOfPoints > );

        virtual ~Polygon() = default;

        Point< Type > operator[]( unsigned int index ) const;
        Point< Type > & operator[]( unsigned int index );
        unsigned int get_number_of_point() const;

        /// @todo mettre un argument pour la rotation
        void set_rectangle( Rectangle< Type > rectangle ) requires(
            C_Quadrangle< NbOfPoints > );

        std::array< Segment< Type >, NbOfPoints > get_segments() const;
        std::array< Point< Type >, NbOfPoints > get_points() const;
    };

    template < unsigned int NbOfPoints >
    using PolygonF = Polygon< float, NbOfPoints >;
    template < unsigned int NbOfPoints >
    using PolygonI = Polygon< int, NbOfPoints >;
    template < unsigned int NbOfPoints >
    using PolygonU = Polygon< unsigned int, NbOfPoints >;

    template < C_Primitive Type >
    using Quadrangle  = Polygon< Type, 4u >;
    using QuadrangleF = Quadrangle< float >;
    using QuadrangleI = Quadrangle< int >;
    using QuadrangleU = Quadrangle< unsigned int >;

    template < C_Primitive Type >
    using Triangle  = Polygon< Type, 3u >;
    using TriangleF = Triangle< float >;
    using TriangleI = Triangle< int >;
    using TriangleU = Triangle< unsigned int >;

    template < C_Primitive Type, unsigned int NbOfPoints >
    bool is_intersection( Polygon< Type, NbOfPoints > polygonLeft,
                          Polygon< Type, NbOfPoints > polygonRight );

} // namespace math

#include "polygon.tpp"
