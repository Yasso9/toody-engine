#pragma once

#include "vector2.hpp"

#include <cmath>
#include <iostream>

#include "maths/numerics.hpp"
#include "tools/assertion.hpp"

namespace math
{
    template< cPrimitive Type >
    Vector2< Type >::Vector2( sf::Vector2< Type > const & sfmlVector )
      : x( sfmlVector.x ), y( sfmlVector.y )
    {}

    template< cPrimitive Type >
    Vector2< Type >::Vector2( ImVec2 const & imGuiVector )
      : x( static_cast< Type >( imGuiVector.x ) ),
        y( static_cast< Type >( imGuiVector.y ) )
    {}

    template< cPrimitive Type >
    Vector2< Type >::Vector2( Vector2< Type > const & vector2D ) noexcept
      : x( vector2D.x ), y( vector2D.y )
    {}

    template< cPrimitive Type >
    Vector2< Type >::Vector2( Vector2< Type > && vector2D ) noexcept
      : x( std::exchange( vector2D.x, {} ) ),
        y( std::exchange( vector2D.y, {} ) )
    {}

    template< cPrimitive Type >
    Vector2< Type > & Vector2< Type >::operator= (
        Vector2< Type > const & vector2D ) noexcept
    {
        return *this = Vector2< Type > { vector2D };
    }

    template< cPrimitive Type >
    Vector2< Type > & Vector2< Type >::operator= (
        Vector2< Type > && vector2D ) noexcept
    {
        std::swap( this->x, vector2D.x );
        std::swap( this->y, vector2D.y );
        return *this;
    }

    /* ************************************************************************
    *********************** ASSIGNEMENT OPERATOR ******************************
    ************************************************************************ */

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::operator-= ( Vector2< Type > rhs )
    {
        return *this = *this - rhs;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::operator+= ( Vector2< Type > rhs )
    {
        return *this = *this + rhs;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::operator*= ( Vector2< Type > rhs )
    {
        return *this = *this * rhs;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::operator/= ( Vector2< Type > rhs )
    {
        return *this = *this / rhs;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::operator/= ( float factor )
    {
        return *this = *this / factor;
    }

    template< cPrimitive Type >
    Vector2< Type >::operator sf::Vector2< Type > () const
    {
        return sf::Vector2< Type > { this->x, this->y };
    }

    /* ********************************************************************
    ******************************* CAST **********************************
    ******************************************************************** */

    template< cPrimitive Type >
    Vector2< Type >::operator ImVec2 () const
        requires ( std::is_same_v< Type, decltype( ImVec2::x ) > )
    {
        return ImVec2 { static_cast< float >( this->x ),
                        static_cast< float >( this->y ) };
    }

    template< cPrimitive Type >
    template< cPrimitive OtherType >
    requires ( not std::is_same_v< Type, OtherType > )

    Vector2< Type >::operator Vector2< OtherType > () const
    {
        return { static_cast< OtherType >( this->x ),
                 static_cast< OtherType >( this->y ) };
    }

    template< cPrimitive Type >
    Vector2< float > Vector2< Type >::to_float() const
        requires ( not std::is_same_v< Type, float > )
    {
        return static_cast< Vector2< float > >( *this );
    }

    template< cPrimitive Type >
    Vector2< unsigned int > Vector2< Type >::to_u_int() const
        requires ( not std::is_same_v< Type, unsigned int > )
    {
        return static_cast< Vector2< unsigned int > >( *this );
    }

    template< cPrimitive Type >
    Vector2< int > Vector2< Type >::to_int() const
        requires ( not std::is_same_v< Type, int > )
    {
        return static_cast< Vector2< int > >( *this );
    }

    template< cPrimitive Type >
    Vector2< std::size_t > Vector2< Type >::to_size_t() const
        requires ( not std::is_same_v< Type, std::size_t > )
    {
        return static_cast< Vector2< std::size_t > >( *this );
    }

    template< cPrimitive Type >
    Point< Type > Vector2< Type >::to_point() const
    {
        return Point< Type > { this->x, this->y };
    }

    /* ********************************************************************
     **************************** METHODS *********************************
     ******************************************************************* */

    template< cPrimitive Type >
    bool Vector2< Type >::is_whole() const
    {
        return math::is_whole_number( this->x )
               && math::is_whole_number( this->y );
    }

    template< cPrimitive Type >
    Type Vector2< Type >::get_max() const
    {
        return std::max( this->x, this->y );
    }

    template< cPrimitive Type >
    Type Vector2< Type >::get_min() const
    {
        return std::min( this->x, this->y );
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::get_x_axis() const
    {
        return Vector2< Type > { this->x, 0 };
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::get_y_axis() const
    {
        return Vector2< Type > { 0, this->y };
    }

    template< cPrimitive Type >
    Type Vector2< Type >::operator[] ( std::size_t index ) const
    {
        switch ( index )
        {
        case 0 :
            return this->x;
            break;
        case 1 :
            return this->y;
            break;
        default :
            ASSERTION( false, "Vector index out of range" );
            break;
        }
        return 0.f;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::floor()
    {
        return *this = math::floor( *this );
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::round()
    {
        return *this = math::round( *this );
    }

    template< cPrimitive Type >
    float Vector2< Type >::get_length() const
    {
        return std::sqrt( ( this->x * this->x ) + ( this->y * this->y ) );
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::get_norm() const
    {
        float vectorLength { this->get_length() };

        if ( vectorLength == 0 )
        {
            return *this;
        }

        return *this / vectorLength;
    }

    template< cPrimitive Type >
    Vector2< Type > Vector2< Type >::normalize()
    {
        return *this = this->get_norm();
    }

    template< cPrimitive Type >
    Vector2< Type > max ( Vector2< Type > const & lhs,
                          Vector2< Type > const & rhs )
    {
        return Vector2< Type > { std::max( lhs.x, rhs.x ),
                                 std::max( lhs.y, rhs.y ) };
    }

    template< cPrimitive Type >
    Vector2< Type > min ( Vector2< Type > const & lhs,
                          Vector2< Type > const & rhs )
    {
        return Vector2< Type > { std::min( lhs.x, rhs.x ),
                                 std::min( lhs.y, rhs.y ) };
    }

    /* ********************************************************************
     *************************** STREAM OPERATOR **************************
     ******************************************************************* */

    template< cPrimitive Type >
    std::ostream & operator<< ( std::ostream &          stream,
                                Vector2< Type > const & vector )
    {
        return stream << "( " << vector.x << ", " << vector.y << " )";
    }

    template< cPrimitive Type >
    std::istream & operator>> ( std::istream &    stream,
                                Vector2< Type > & vector )
    {
        stream::ignore_next( stream, '(' );
        stream >> vector.x;
        stream::ignore_next( stream, ',' );
        stream >> vector.y;
        stream::ignore_next( stream, ')' );

        return stream;
    }

    /* ************************************************************************
    ************************** VECTOR FUNCTIONS *******************************
    ************************************************************************ */

    template< cPrimitive Type >
    bool is_inside ( Vector2< Type > const & value,
                     Vector2< Type > const & position,
                     Vector2< Type > const & size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

    template< cPrimitive Type >
    Vector2< Type > floor ( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > {
            static_cast< Type >( std::floor( vector2D.x ) ),
            static_cast< Type >( std::floor( vector2D.y ) ) };
    }

    template< cPrimitive Type >
    Vector2< Type > round ( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > {
            static_cast< Type >( std::round( vector2D.x ) ),
            static_cast< Type >( std::round( vector2D.y ) ) };
    }

    /* ************************************************************************
    ************************** VECTOR VECTOR ********************************
    ************************************************************************ */

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator* ( Vector2< TypeLeft > const &  vector2DLeft,
                                    Vector2< TypeRight > const & vector2DRight )
    {
        return { vector2DLeft.x * static_cast< TypeLeft >( vector2DRight.x ),
                 vector2DLeft.y * static_cast< TypeLeft >( vector2DRight.y ) };
    }

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator/ ( Vector2< TypeLeft > const &  vector2DLeft,
                                    Vector2< TypeRight > const & vector2DRight )
    {
        Vector2< TypeLeft > vector2DRightCast {
            static_cast< Vector2< TypeLeft > >( vector2DRight ) };
        if ( vector2DRightCast.x == 0 || vector2DRightCast.y == 0 )
        {
            throw std::logic_error { "Division by Zero" };
        }

        return { vector2DLeft.x / vector2DRightCast.x,
                 vector2DLeft.y / vector2DRightCast.y };
    }

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator+ ( Vector2< TypeLeft > const &  vector2DLeft,
                                    Vector2< TypeRight > const & vector2DRight )
    {
        return { vector2DLeft.x + static_cast< TypeLeft >( vector2DRight.x ),
                 vector2DLeft.y + static_cast< TypeLeft >( vector2DRight.y ) };
    }

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator- ( Vector2< TypeLeft > const &  vector2DLeft,
                                    Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft + ( -vector2DRight );
    }

    /* ************************************************************************
    ************************** VECTOR * FACTOR ********************************
    ************************************************************************ */

    template< cPrimitive VectorType, cPrimitive FactorType >
    Vector2< VectorType > operator* ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor )
    {
        return vector2D * Vector2< FactorType > { factor, factor };
    }

    template< cPrimitive VectorType, cPrimitive FactorType >
    Vector2< VectorType > operator/ ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor )
    {
        return vector2D / Vector2< FactorType > { factor, factor };
    }

    template< cPrimitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                int const &             modulo )
    {
        Vector2< int > const vector2DCastToInt {
            static_cast< Vector2< int > >( vector2D ) };

        return static_cast< Vector2< Type > >( Vector2< int > {
            vector2DCastToInt.x % modulo, vector2DCastToInt.y % modulo } );
    }

    template< cPrimitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                unsigned int const &    modulo )
    {
        return vector2D % static_cast< int >( modulo );
    }

    /* ************************************************************************
    **************************** SIGN OPERATOR ********************************
    ************************************************************************ */

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    bool operator== ( Vector2< TypeLeft > const &  vector2DLeft,
                      Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft.x == static_cast< TypeLeft >( vector2DRight.x )
               && vector2DLeft.y == static_cast< TypeLeft >( vector2DRight.y );
    }

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    bool operator<( Vector2< TypeLeft > const &  vector2DLeft,
                    Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft.x < static_cast< TypeLeft >( vector2DRight.x )
               && vector2DLeft.y < static_cast< TypeLeft >( vector2DRight.y );
    }

    template< cPrimitive Type >
    bool operator> ( Vector2< Type > const & vector2DLeft,
                     Vector2< Type > const & vector2DRight )
    {
        return vector2DLeft.x < vector2DRight.x
               && vector2DLeft.y < vector2DRight.y;
    }

    /* ************************************************************************
    ************************** UNARY VECTOR ********************************
    ************************************************************************ */

    template< cRelativePrimitive Type >
    Vector2< Type > operator- ( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > { -vector2D.x, -vector2D.y };
    }
}  // namespace math
