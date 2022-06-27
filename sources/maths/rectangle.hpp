#pragma once

#include "tools/concepts.hpp"

#include "maths/segment.hpp"
#include "maths/vector2.hpp"

namespace math
{
    template < C_Primitive Type >
    class Rectangle
    {
      public:
        Point< Type > position;
        Vector2< Type > size;

        Rectangle() : Rectangle( 0.f, 0.f, 0.f, 0.f ) {}
        Rectangle( Vector2< Type > aPosition, Vector2< Type > aSize )
          : Rectangle( aPosition.x, aPosition.y, aSize.x, aSize.y )
        {}
        Rectangle( Type x, Type y, Type width, Type height )
          : position( x, y ), size( width, height )
        {}
    };

    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;

    template < C_Primitive Type >
    class Quadrangle
    {
      public:
        Point< Type > topLeftPosition; // A
        Point< Type > topRightPosition; // B
        Point< Type > bottomRightPosition; // C
        Point< Type > bottomLeftPosition; // D

        unsigned int const NUMBER_OF_POINT { 4u };

        Quadrangle()
          : topLeftPosition( 0, 0 ),
            topRightPosition( 0, 0 ),
            bottomRightPosition( 0, 0 ),
            bottomLeftPosition( 0, 0 )
        {}

        Quadrangle( Point< Type > a, Point< Type > b, Point< Type > c,
                    Point< Type > d )
          : topLeftPosition( a ),
            topRightPosition( b ),
            bottomRightPosition( c ),
            bottomLeftPosition( d )
        {}

        Quadrangle( Rectangle< Type > rectangle ) : Quadrangle {}
        {
            this->set_rectangle( rectangle );
        }

        virtual ~Quadrangle() = default;

        virtual Point< Type > operator[]( unsigned int index ) const
        {
            switch ( index )
            {
            case 0u :
                return topLeftPosition;
                break;
            case 1u :
                return topRightPosition;
                break;
            case 2u :
                return bottomRightPosition;
                break;
            case 3u :
                return bottomLeftPosition;
                break;
            default :
                throw std::runtime_error { "Quadrangle index out of range" };
                break;
            }
        }

        /// @todo mettre un argument pour la rotation
        void set_rectangle( Rectangle< Type > rectangle )
        {
            topLeftPosition = rectangle.position;
            topRightPosition =
                topLeftPosition + Vector2< Type > { rectangle.position.x, 0.f };

            bottomRightPosition = topLeftPosition + rectangle.position;
            bottomLeftPosition =
                topLeftPosition + Vector2< Type > { 0.f, rectangle.position.y };
        }

        /// @todo avoir un std array a la place
        std::vector< Segment< Type > > get_segments() const
        {
            std::vector< Segment< Type > > segments {};

            for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT;
                  ++i_point )
            {
                unsigned int i_pointNext { ( i_point + 1 ) % NUMBER_OF_POINT };
                segments.push_back(
                    { ( *this )[i_point], ( *this )[i_pointNext] } );
            }

            return segments;
        }
        std::vector< Point< Type > > get_points() const
        {
            std::vector< Point< Type > > points {};

            for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT;
                  ++i_point )
            {
                points.push_back( ( *this )[i_point] );
            }

            return points;
        }
    };

    using QuadrangleF = Quadrangle< float >;
    using QuadrangleI = Quadrangle< int >;
    using QuadrangleU = Quadrangle< unsigned int >;

    template < C_Primitive Type >
    bool is_intersection( Quadrangle< Type > quadrangleA,
                          Quadrangle< Type > quadrangleB )
    {
        for ( Point< Type > pointA : quadrangleA.get_points() )
        {
            if ( pointA.is_inside( quadrangleB ) )
            {
                return true;
            }
        }
        for ( Point< Type > pointB : quadrangleB.get_points() )
        {
            if ( pointB.is_inside( quadrangleA ) )
            {
                return true;
            }
        }

        return false;
    }
} // namespace math

#include "rectangle.tpp"