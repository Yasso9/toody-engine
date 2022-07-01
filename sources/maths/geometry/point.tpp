#pragma once

#include "point.hpp"

namespace math
{
    /* ************************************************************************
    ****************************** METHODS ************************************
    ************************************************************************ */

    template < C_Primitive Type >
    Point< Type >::Point( Vector2< Type > const & vector2D )
      : Vector2< Type >( vector2D )
    {}
    template < C_Primitive Type >
    Point< Type >::Point( Vector2< Type > && vector2D ) noexcept
      : Vector2< Type >( std::exchange( vector2D, {} ) )
    {}

    template < C_Primitive Type >
    Point< Type > & Point< Type >::operator=( Vector2< Type > const & vector2D )
    {
        return *this = Point< Type > { vector2D };
    }

    template < C_Primitive Type >
    Point< Type > & Point< Type >::operator=(
        Vector2< Type > && vector2D ) noexcept
    {
        std::swap( this->x, vector2D.x );
        std::swap( this->y, vector2D.y );
        return *this;
    }

    template < C_Primitive Type >
    Point< Type >::operator Vector2< Type >() const
    {
        return Vector2< Type > { this->x, this->y };
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
    template < unsigned int NB_OF_POINT >
    bool Point< Type >::is_inside( Polygon< Type, NB_OF_POINT > polygon ) const
    {
        /// @todo recuperer ça dynamiquement
        constexpr float LIMIT_ZONE { 10000.f };

        // Create a point for line segment from p to infinite
        Vector2< Type > extremePoint { LIMIT_ZONE, this->y };
        Segment< Type > segmentToCheck { *this, extremePoint };

        unsigned int numberOfIntersection { 0u };
        for ( Segment< Type > segment : polygon.get_segments() )
        {
            if ( is_intersection( segmentToCheck, segment ) )
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
    E_Orientation get_orientation( Vector2< Type > pointA,
                                   Vector2< Type > pointB,
                                   Vector2< Type > pointC )
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

        ASSERTION( false );
        return E_Orientation::Unkown;
    }

    template < C_Primitive Type >
    bool are_collinear( Vector2< Type > pointA, Vector2< Type > pointB,
                        Vector2< Type > pointC )
    {
        return E_Orientation::Collinear
               == get_orientation( pointA, pointB, pointC );
    }
} // namespace math