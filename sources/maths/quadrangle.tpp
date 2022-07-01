#pragma once
#include "quadrangle.hpp"

namespace math
{
    template < C_Primitive Type >
    Quadrangle< Type >::Quadrangle()
      : topLeftPosition( 0, 0 ),
        topRightPosition( 0, 0 ),
        bottomRightPosition( 0, 0 ),
        bottomLeftPosition( 0, 0 )
    {}

    template < C_Primitive Type >
    Quadrangle< Type >::Quadrangle( Point< Type > a, Point< Type > b,
                                    Point< Type > c, Point< Type > d )
      : topLeftPosition( a ),
        topRightPosition( b ),
        bottomRightPosition( c ),
        bottomLeftPosition( d )
    {}

    template < C_Primitive Type >
    Quadrangle< Type >::Quadrangle( Rectangle< Type > rectangle )
      : Quadrangle {}
    {
        this->set_rectangle( rectangle );
    }

    template < C_Primitive Type >
    Point< Type > Quadrangle< Type >::operator[]( unsigned int index ) const
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

    template < C_Primitive Type >
    unsigned int Quadrangle< Type >::get_number_of_point() const
    {
        return NUMBER_OF_POINT;
    }

    template < C_Primitive Type >
    void Quadrangle< Type >::set_rectangle( Rectangle< Type > rectangle )
    {
        topLeftPosition = rectangle.position;
        topRightPosition =
            topLeftPosition + Vector2< Type > { rectangle.position.x, 0.f };

        bottomRightPosition = topLeftPosition + rectangle.position;
        bottomLeftPosition =
            topLeftPosition + Vector2< Type > { 0.f, rectangle.position.y };
    }

    template < C_Primitive Type >
    std::vector< Segment< Type > > Quadrangle< Type >::get_segments() const
    {
        std::vector< Segment< Type > > segments {};

        for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT; ++i_point )
        {
            unsigned int i_pointNext { ( i_point + 1 ) % NUMBER_OF_POINT };
            segments.push_back(
                { ( *this )[i_point], ( *this )[i_pointNext] } );
        }

        return segments;
    }
    template < C_Primitive Type >
    std::vector< Point< Type > > Quadrangle< Type >::get_points() const
    {
        std::vector< Point< Type > > points {};

        for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT; ++i_point )
        {
            points.push_back( ( *this )[i_point] );
        }

        return points;
    }

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