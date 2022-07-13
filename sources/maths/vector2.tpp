#pragma once
#include "vector2.hpp"

#include <cmath>
#include <iostream>

#include "tools/assertion.hpp"

namespace math
{
    template < C_Primitive Type >
    Vector2< Type >::Vector2( sf::Vector2< Type > const & sfmlVector )
      : x( sfmlVector.x ), y( sfmlVector.y )
    {}
    template < C_Primitive Type >
    Vector2< Type >::Vector2( ImVec2 const & imGuiVector )
      : x( static_cast< Type >( imGuiVector.x ) ),
        y( static_cast< Type >( imGuiVector.y ) )
    {}

    template < C_Primitive Type >
    Vector2< Type >::Vector2( Vector2< Type > const & vector2D )
      : x( vector2D.x ), y( vector2D.y )
    {}
    template < C_Primitive Type >
    Vector2< Type >::Vector2( Vector2< Type > && vector2D ) noexcept
      : x( std::exchange( vector2D.x, {} ) ),
        y( std::exchange( vector2D.y, {} ) )
    {}
    template < C_Primitive Type >
    Vector2< Type > & Vector2< Type >::operator=(
        Vector2< Type > const & vector2D )
    {
        return *this = Vector2< Type > { vector2D };
    }
    template < C_Primitive Type >
    Vector2< Type > & Vector2< Type >::operator=(
        Vector2< Type > && vector2D ) noexcept
    {
        std::swap( this->x, vector2D.x );
        std::swap( this->y, vector2D.y );
        return *this;
    }

    template < C_Primitive Type >
    Vector2< Type >::operator sf::Vector2< Type >() const
    {
        return sf::Vector2< Type > { this->x, this->y };
    }
    template < C_Primitive Type >
    Vector2< Type >::operator ImVec2() const
        requires( std::is_same_v< Type, decltype( ImVec2::x ) > )
    {
        return ImVec2 { static_cast< float >( this->x ),
                        static_cast< float >( this->y ) };
    }

    template < C_Primitive Type >
    template < C_Primitive OtherType >
        requires( not std::is_same_v< Type, OtherType > )
    Vector2< Type >::operator Vector2< OtherType >() const
    {
        return { static_cast< OtherType >( this->x ),
                 static_cast< OtherType >( this->y ) };
    }

    template < C_Primitive Type >
    Vector2< float > Vector2< Type >::to_float() const
        requires( not std::is_same_v< Type, float > )
    {
        return static_cast< Vector2< float > >( *this );
    }
    template < C_Primitive Type >
    Vector2< unsigned int > Vector2< Type >::to_u_int() const
        requires( not std::is_same_v< Type, unsigned int > )
    {
        return static_cast< Vector2< unsigned int > >( *this );
    }
    template < C_Primitive Type >
    Vector2< int > Vector2< Type >::to_int() const
        requires( not std::is_same_v< Type, int > )
    {
        return static_cast< Vector2< int > >( *this );
    }
    template < C_Primitive Type >
    Vector2< std::size_t > Vector2< Type >::to_size_t() const
        requires( not std::is_same_v< Type, std::size_t > )
    {
        return static_cast< Vector2< std::size_t > >( *this );
    }

    template < C_Primitive Type >
    Type Vector2< Type >::operator[]( std::size_t index ) const
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

    template < C_Primitive Type >
    Vector2< Type > Vector2< Type >::floor()
    {
        return *this = math::floor( *this );
    }
    template < C_Primitive Type >
    Vector2< Type > Vector2< Type >::round()
    {
        return *this = math::round( *this );
    }

    template < C_Primitive Type >
    float Vector2< Type >::get_length() const
    {
        return std::sqrt( ( this->x * this->x ) + ( this->y * this->y ) );
    }
    template < C_Primitive Type >
    Vector2< Type > Vector2< Type >::normalize()
    {
        float const vectorLength { this->get_length() };

        if ( vectorLength != 0 )
        {
            *this /= vectorLength;
        }

        return *this;
    }

    /* ************************************************************************
    ************************** VECTOR FUNCTIONS *******************************
    ************************************************************************ */

    template < C_Primitive Type >
    bool is_inside( Vector2< Type > const & value,
                    Vector2< Type > const & position,
                    Vector2< Type > const & size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

    template < C_Primitive Type >
    Vector2< Type > floor( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > {
            static_cast< Type >( std::floor( vector2D.x ) ),
            static_cast< Type >( std::floor( vector2D.y ) )
        };
    }
    template < C_Primitive Type >
    Vector2< Type > round( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > {
            static_cast< Type >( std::round( vector2D.x ) ),
            static_cast< Type >( std::round( vector2D.y ) )
        };
    }

    template < C_Primitive Type >
    std::ostream & operator<<( std::ostream & stream,
                               Vector2< Type > const & vector2D )
    {
        return stream << "( " << vector2D.x << ", " << vector2D.y << " )";
    }

    /* ************************************************************************
    ************************** VECTOR VECTOR ********************************
    ************************************************************************ */

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator*( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight )
    {
        return { vector2DLeft.x * static_cast< TypeLeft >( vector2DRight.x ),
                 vector2DLeft.y * static_cast< TypeLeft >( vector2DRight.y ) };
    }
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator/( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight )
    {
        Vector2< TypeLeft > vector2DRightCast {
            static_cast< Vector2< TypeLeft > >( vector2DRight )
        };
        if ( vector2DRightCast.x == 0 || vector2DRightCast.y == 0 )
        {
            throw std::logic_error { "Division by Zero" };
        }

        return { vector2DLeft.x / vector2DRightCast.x,
                 vector2DLeft.y / vector2DRightCast.y };
    }
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator+( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight )
    {
        return { vector2DLeft.x + static_cast< TypeLeft >( vector2DRight.x ),
                 vector2DLeft.y + static_cast< TypeLeft >( vector2DRight.y ) };
    }
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator-( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft + ( -vector2DRight );
    }

    /* ************************************************************************
    ************************** VECTOR * FACTOR ********************************
    ************************************************************************ */

    template < C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator*( Vector2< VectorType > const & vector2D,
                                     FactorType const & factor )
    {
        return vector2D * Vector2< FactorType > { factor, factor };
    }
    template < C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator/( Vector2< VectorType > const & vector2D,
                                     FactorType const & factor )
    {
        return vector2D / Vector2< FactorType > { factor, factor };
    }

    template < C_Primitive Type >
    Vector2< Type > operator%( Vector2< Type > const & vector2D,
                               int const & modulo )
    {
        Vector2< int > const vector2DCastToInt { static_cast< Vector2< int > >(
            vector2D ) };

        return static_cast< Vector2< Type > >(
            Vector2< int > { vector2DCastToInt.x % modulo,
                             vector2DCastToInt.y % modulo } );
    }

    template < C_Primitive Type >
    Vector2< Type > operator%( Vector2< Type > const & vector2D,
                               unsigned int const & modulo )
    {
        return vector2D % static_cast< int >( modulo );
    }

    /* ************************************************************************
    **************************** SIGN OPERATOR ********************************
    ************************************************************************ */

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator==( Vector2< TypeLeft > const & vector2DLeft,
                     Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft.x == static_cast< TypeLeft >( vector2DRight.x )
               && vector2DLeft.y == static_cast< TypeLeft >( vector2DRight.y );
    }

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator<( Vector2< TypeLeft > const & vector2DLeft,
                    Vector2< TypeRight > const & vector2DRight )
    {
        return vector2DLeft.x < static_cast< TypeLeft >( vector2DRight.x )
               && vector2DLeft.y < static_cast< TypeLeft >( vector2DRight.y );
    }

    /* ************************************************************************
    ************************** UNARY VECTOR ********************************
    ************************************************************************ */

    template < C_RelativePrimitive Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2D )
    {
        return Vector2< Type > { -vector2D.x, -vector2D.y };
    }

    template < C_RelativePrimitive Type >
    Vector2< Type > operator-=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight )
    {
        return vector2DLeft = vector2DLeft - vector2DRight;
    }
    template < C_RelativePrimitive Type >
    Vector2< Type > operator+=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight )
    {
        return vector2DLeft = vector2DLeft + vector2DRight;
    }
    template < C_RelativePrimitive Type >
    Vector2< Type > operator/=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight )
    {
        return vector2DLeft = vector2DLeft / vector2DRight;
    }
    template < C_RelativePrimitive Type >
    Vector2< Type > operator/=( Vector2< Type > & vector2D, float factor )
    {
        return vector2D = vector2D / factor;
    }
} // namespace math