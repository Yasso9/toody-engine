#pragma once

#include "point.hpp"

namespace math
{
    /* ************************************************************************
    ****************************** METHODS ************************************
    ************************************************************************ */

    template < C_Primitive Type >
    Point< Type >::Point( Vector2< Type > const & vector2D ) noexcept
      : Vector2< Type >( vector2D )
    {}

    template < C_Primitive Type >
    Point< Type >::operator Vector2< Type >() const
    {
        return Vector2< Type > { this->x, this->y };
    }

    template < C_Primitive Type >
    template < C_Primitive OtherType >
        requires( not std::is_same_v< Type, OtherType > )
    Point< Type >::operator Point< OtherType >() const
    {
        return { static_cast< OtherType >( this->x ),
                 static_cast< OtherType >( this->y ) };
    }

    template < C_Primitive Type >
    Point< float > Point< Type >::to_float() const
        requires( not std::is_same_v< Type, float > )
    {
        return static_cast< Point< float > >( *this );
    }
    template < C_Primitive Type >
    Point< unsigned int > Point< Type >::to_u_int() const
        requires( not std::is_same_v< Type, unsigned int > )
    {
        return static_cast< Point< unsigned int > >( *this );
    }
    template < C_Primitive Type >
    Point< int > Point< Type >::to_int() const
        requires( not std::is_same_v< Type, int > )
    {
        return static_cast< Point< int > >( *this );
    }
    template < C_Primitive Type >
    Point< std::size_t > Point< Type >::to_size_t() const
        requires( not std::is_same_v< Type, std::size_t > )
    {
        return static_cast< Point< std::size_t > >( *this );
    }

    template < C_Primitive Type >
    bool Point< Type >::is_inside( Segment< Type > segment ) const
    {
        return this->x <= std::max( segment.pointA.x, segment.pointB.x )
               && this->x >= std::min( segment.pointA.x, segment.pointB.x )
               && this->y <= std::max( segment.pointA.y, segment.pointB.y )
               && this->y >= std::min( segment.pointA.y, segment.pointB.y );
    }
    template < C_Primitive Type >
    bool Point< Type >::is_inside( Point< Type > position,
                                   Vector2< Type > size ) const
    {
        return this->is_inside( Rectangle< Type > { position, size } );
    }
    template < C_Primitive Type >
    bool Point< Type >::is_inside( Rectangle< Type > rectangle ) const
    {
        return ( this->x >= rectangle.position.x
                 && this->x < rectangle.position.x + rectangle.size.x
                 && this->y >= rectangle.position.y
                 && this->y < rectangle.position.y + rectangle.size.y );
    }
    template < C_Primitive Type >
    bool Point< Type >::is_inside( Polygon< Type > polygon ) const
    {
        /// @todo recuperer ça dynamiquement
        constexpr float LIMIT_ZONE { 10000.f };

        // Create a point for line segment from p to infinite
        Point< Type > extremePoint { LIMIT_ZONE, this->y };
        Segment< Type > segmentToCheck { *this, extremePoint };

        unsigned int numberOfIntersection { 0u };
        for ( Segment< Type > segment : polygon.get_segments() )
        {
            if ( segmentToCheck.is_intersected_by( segment ) )
            {
                ++numberOfIntersection;
            }
        }

        return numberOfIntersection == 1;
    }

    /* ************************************************************************
    ****************************** FUNCTIONS **********************************
    ************************************************************************ */

    template < C_Primitive Type >
    E_Orientation get_orientation( Point< Type > pointA, Point< Type > pointB,
                                   Point< Type > pointC )
    {
        float value = ( ( pointB.y - pointA.y ) * ( pointC.x - pointB.x ) )
                      - ( ( pointB.x - pointA.x ) * ( pointC.y - pointB.y ) );

        if ( value == 0.f )
        {
            return E_Orientation::Collinear;
        }
        else if ( value > 0.f )
        {
            return E_Orientation::ClockWise;
        }
        else if ( value < 0.f )
        {
            return E_Orientation::CounterClockWise;
        }

        ASSERTION( false, "" );
        return E_Orientation::Unkown;
    }

    template < C_Primitive Type >
    bool are_collinear( Point< Type > pointA, Point< Type > pointB,
                        Point< Type > pointC )
    {
        return E_Orientation::Collinear
               == get_orientation( pointA, pointB, pointC );
    }
} // namespace math