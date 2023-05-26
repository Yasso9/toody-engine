#pragma once

#include "point.hpp"

#include "maths/geometry/line.hpp"

namespace math
{
    /* ************************************************************************
    ****************************** METHODS ************************************
    ************************************************************************ */

    template< cPrimitive Type >
    Point< Type >::Point( Vector2< Type > const & vector2D ) noexcept
      : Vector2< Type >( vector2D )
    {}

    template< cPrimitive Type >
    Point< Type >::operator Vector2< Type > () const
    {
        return Vector2< Type > { this->x, this->y };
    }

    template< cPrimitive Type >
    template< cPrimitive OtherType >
    requires ( not std::is_same_v< Type, OtherType > )

    Point< Type >::operator Point< OtherType > () const
    {
        return { static_cast< OtherType >( this->x ),
                 static_cast< OtherType >( this->y ) };
    }

    template< cPrimitive Type >
    Vector2< Type > Point< Type >::to_vector() const
    {
        return Vector2< Type > { this->x, this->y };
    }

    template< cPrimitive Type >
    Point< float > Point< Type >::to_float() const
        requires ( not std::is_same_v< Type, float > )
    {
        return static_cast< Point< float > >( *this );
    }

    template< cPrimitive Type >
    Point< unsigned int > Point< Type >::to_u_int() const
        requires ( not std::is_same_v< Type, unsigned int > )
    {
        return static_cast< Point< unsigned int > >( *this );
    }

    template< cPrimitive Type >
    Point< int > Point< Type >::to_int() const
        requires ( not std::is_same_v< Type, int > )
    {
        return static_cast< Point< int > >( *this );
    }

    template< cPrimitive Type >
    Point< std::size_t > Point< Type >::to_size_t() const
        requires ( not std::is_same_v< Type, std::size_t > )
    {
        return static_cast< Point< std::size_t > >( *this );
    }

    template< cPrimitive Type >
    bool Point< Type >::is_inside( Line< Type > segment ) const
    {
        return this->x <= std::max( segment.pointA.x, segment.pointB.x )
               && this->x >= std::min( segment.pointA.x, segment.pointB.x )
               && this->y <= std::max( segment.pointA.y, segment.pointB.y )
               && this->y >= std::min( segment.pointA.y, segment.pointB.y );
    }

    template< cPrimitive Type >
    bool Point< Type >::is_inside( Point< Type >   position,
                                   Vector2< Type > size ) const
    {
        return ( this->x >= position.x && this->x < position.x + size.x
                 && this->y >= position.y && this->y < position.y + size.y );
    }

    template< cPrimitive Type >
    bool Point< Type >::is_inside( Rectangle< Type > rectangle ) const
    {
        return this->is_inside( rectangle.position, rectangle.size );
    }

    template< cPrimitive Type >
    bool Point< Type >::is_inside( Polygon< Type > polygon ) const
    {
        constexpr float LIMIT_ZONE { 10000.f };

        // Create a point for line segment from p to infinite
        Point< Type > extremePoint { LIMIT_ZONE, this->y };
        Line< Type >  segmentToCheck { *this, extremePoint };

        unsigned int numberOfIntersection { 0u };
        for ( auto segment : polygon.get_segments() )
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

    template< cPrimitive Type >
    E_Orientation get_orientation ( Point< Type > pointA, Point< Type > pointB,
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

    template< cPrimitive Type >
    bool are_collinear ( Point< Type > pointA, Point< Type > pointB,
                         Point< Type > pointC )
    {
        return E_Orientation::Collinear
               == get_orientation( pointA, pointB, pointC );
    }
}  // namespace math
