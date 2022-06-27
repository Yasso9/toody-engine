#pragma once

#include "tools/concepts.hpp"

#include "vector2.hpp"

namespace math
{
    template < C_Primitive Type >
    class Rectangle
    {
      public:
        Vector2< Type > position;
        Vector2< Type > size;

        Rectangle() : Rectangle( 0.f, 0.f, 0.f, 0.f ) {}
        Rectangle( Vector2< Type > aPosition, Vector2< Type > aSize )
          : position( aPosition ), size( aSize )
        {}
        Rectangle( Type x, Type y, Type width, Type height )
          : position( x, y ), size( width, height )
        {}

      private:
        // float m_rotation;
    };
    using RectangleF = Rectangle< float >;
    using RectangleI = Rectangle< int >;
    using RectangleU = Rectangle< unsigned int >;

    struct S_Segment
    {
        Vector2F pointA { 0.f, 0.f };
        Vector2F pointB { 0.f, 0.f };

        S_Segment( Vector2F point1, Vector2F point2 )
          : pointA( point1 ), pointB( point2 )
        {}
    };

    template < C_Primitive Type >
    class Quadrangle;

    using QuadrangleF = Quadrangle< float >;
    using QuadrangleI = Quadrangle< int >;
    using QuadrangleU = Quadrangle< unsigned int >;

    template < C_Primitive Type >
    class Quadrangle
    {
      public:
        Vector2< Type > topLeftPosition; // A
        Vector2< Type > topRightPosition; // B
        Vector2< Type > bottomRightPosition; // C
        Vector2< Type > bottomLeftPosition; // D

        unsigned int const NUMBER_OF_POINT { 4u };

        Quadrangle()
          : topLeftPosition( 0, 0 ),
            topRightPosition( 0, 0 ),
            bottomLeftPosition( 0, 0 ),
            bottomRightPosition( 0, 0 )
        {}

        Quadrangle( Vector2< Type > a, Vector2< Type > b, Vector2< Type > c,
                    Vector2< Type > d )
          : topLeftPosition( a ),
            topRightPosition( b ),
            bottomRightPosition( c ),
            bottomLeftPosition( d )
        {}

        Quadrangle( Rectangle< Type > rectangle )
        {
            this->set_rectangle( rectangle );
        }

        virtual ~Quadrangle() = default;

        virtual Vector2< Type > operator[]( unsigned int index ) const
        {
            switch ( index )
            {
            case 0u :
                return topLeftPosition;
                break;
            case 0u :
                return topRightPosition;
                break;
            case 0u :
                return bottomRightPosition;
                break;
            case 0u :
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
        std::vector< S_Segment > get_segments() const
        {
            std::vector< S_Segment > segments {};

            for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT;
                  ++i_point )
            {
                unsigned int i_pointNext { ( i_point + 1 ) % NUMBER_OF_POINT };
                segments.push_back( { this[i_point], this[i_pointNext] } );
            }

            return segments;
        }
        std::vector< Vector2F > get_points() const
        {
            std::vector< Vector2F > points {};

            for ( unsigned int i_point { 0 }; i_point < NUMBER_OF_POINT;
                  ++i_point )
            {
                points.push_back( this[i_point] );
            }

            return points;
        }
    };

    enum class E_Orientation
    {
        Collinear = 0,
        ClockWise,
        CounterClockWise,
        Unkown
    };

    bool is_inside( Vector2F point, S_Segment segment );
    bool is_inside( Vector2F point, S_Segment segment )
    {
        return point.x <= std::max( segment.pointA.x, segment.pointB.x )
               && point.x >= std::min( segment.pointA.x, segment.pointB.x )
               && point.y <= std::max( segment.pointA.y, segment.pointB.y )
               && point.y >= std::min( segment.pointA.y, segment.pointB.y );
    }

    E_Orientation get_orientation( Vector2F pointA, Vector2F pointB,
                                   Vector2F pointC );
    E_Orientation get_orientation( Vector2F pointA, Vector2F pointB,
                                   Vector2F pointC )
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

    bool are_collinear( Vector2F pointA, Vector2F pointB, Vector2F pointC );
    bool are_collinear( Vector2F pointA, Vector2F pointB, Vector2F pointC )
    {
        return E_Orientation::Collinear
               == get_orientation( pointA, pointB, pointC );
    }

    bool is_intersection( S_Segment segment1, S_Segment segment2 );
    bool is_intersection( S_Segment segment1, S_Segment segment2 )
    {
        // Find the four orientations needed for general and
        // special cases
        E_Orientation o1 = get_orientation( segment1.pointA,
                                            segment1.pointB,
                                            segment2.pointA );

        E_Orientation o2 = get_orientation( segment1.pointA,
                                            segment1.pointB,
                                            segment2.pointB );

        E_Orientation o3 = get_orientation( segment2.pointA,
                                            segment2.pointB,
                                            segment1.pointA );

        E_Orientation o4 = get_orientation( segment2.pointA,
                                            segment2.pointB,
                                            segment1.pointB );

        // General case
        if ( o1 != o2 && o3 != o4 )
            return true;

        // Special Cases
        // p1, q1 and p2 are collinear and p2 lies on segment p1q1
        if ( o1 == E_Orientation::Collinear
             && is_inside( segment2.pointA, segment1 ) )
            return true;

        // p1, q1 and p2 are collinear and q2 lies on segment p1q1
        if ( o2 == E_Orientation::Collinear
             && is_inside( segment2.pointB, segment1 ) )
            return true;

        // p2, q2 and p1 are collinear and p1 lies on segment p2q2
        if ( o3 == E_Orientation::Collinear
             && is_inside( segment1.pointA, segment2 ) )
            return true;

        // p2, q2 and q1 are collinear and q1 lies on segment p2q2
        if ( o4 == E_Orientation::Collinear
             && is_inside( segment1.pointB, segment2 ) )
            return true;

        return false; // Doesn't fall in any of the above cases
    }

    // Returns true if the point p lies inside the polygon[] with n vertices
    bool is_inside( Vector2F point, QuadrangleF quadrangle );
    bool is_inside( Vector2F point, QuadrangleF quadrangle )
    {
        /// @todo recuperer ça dynamiquement
        constexpr float LIMIT_ZONE { 10000.f };

        // Create a point for line segment from p to infinite
        Vector2F extremePoint { LIMIT_ZONE, point.y };
        S_Segment segmentToCheck { point, extremePoint };

        unsigned int numberOfIntersection { 0u };
        for ( S_Segment segment : quadrangle.get_segments() )
        {
            if ( is_intersection( segmentToCheck, segment ) )
            {
                ++numberOfIntersection;
            }
        }

        return numberOfIntersection == 1;
    }

    template < C_Primitive Type >
    bool is_intersection( Quadrangle< Type > quadrangleA,
                          Quadrangle< Type > quadrangleB )
    {
        for ( Vector2F pointA : quadrangleA.get_points() )
        {
            if ( pointA.is_inside( quadrangleB ) )
                return true;
        }
        for ( Vector2F pointB : quadrangleB.get_points() )
        {
            if ( pointB.is_inside( quadrangleA ) )
                return true;
        }

        return false;
    }
} // namespace math

#include "rectangle.tpp"