#pragma once

#include <iostream>
#include <utility>

#include "graphics2D/sfml.hpp"
#include "tools/concepts.hpp"
#include "tools/imgui.hpp"

namespace math
{
    // Vector2 is used in Rectangle, we need to forward declare
    template < C_Primitive Type >
    class Rectangle;

    template < C_Primitive Type >
    class Vector2
    {
      public:
        Type x;
        Type y;

        constexpr Vector2() : x( 0 ), y( 0 ) {}
        constexpr Vector2( Type const & xAxisValue, Type const & yAxisValue )
          : x( xAxisValue ), y( yAxisValue )
        {}
        Vector2( sf::Vector2< Type > const & sfmlVector );
        Vector2( ImVec2 const & imGuiVector );
        constexpr virtual ~Vector2() {};

        /// @todo les mettre dans le cpp et ne pas mettre de type explicite mais plutot Vector2F ou Vector2U
        /// @brief copy constructor
        Vector2( Vector2< Type > const & vector2D )
          : x( vector2D.x ), y( vector2D.y )
        {}
        /// @brief move constructor
        Vector2( Vector2< Type > && vector2D ) noexcept
          : x( std::exchange( vector2D.x, {} ) ),
            y( std::exchange( vector2D.y, {} ) )
        {}
        /// @brief copy assignement
        Vector2< Type > & operator=( Vector2< Type > const & vector2D )
        {
            return *this = Vector2< Type > { vector2D };
        }
        /// @brief move assignement
        Vector2< Type > & operator=( Vector2< Type > && vector2D ) noexcept
        {
            std::swap( this->x, vector2D.x );
            std::swap( this->y, vector2D.y );
            return *this;
        }

        operator sf::Vector2< Type >() const;
        operator ImVec2() const;
        /// @todo les mettre dans le cpp et ne pas mettre de type explicite mais plutot Vector2F ou Vector2U
        operator Vector2< float >() const
        {
            return { static_cast< float >( this->x ),
                     static_cast< float >( this->y ) };
        }
        operator Vector2< unsigned int >() const
        {
            return { static_cast< unsigned int >( this->x ),
                     static_cast< unsigned int >( this->y ) };
        }
        operator Vector2< int >() const
        {
            return { static_cast< int >( this->x ),
                     static_cast< int >( this->y ) };
        }

        virtual Type operator[]( std::size_t index ) const;

        bool is_inside( Rectangle< Type > const & rectangle ) const;
        bool is_inside( Vector2< Type > const & position,
                        Vector2< Type > const & size ) const;
        void floor();
        void round();
    };

    template < C_Primitive Type >
    bool is_inside( Vector2< Type > const & value,
                    Vector2< Type > const & position,
                    Vector2< Type > const & size );

    template < C_Primitive Type >
    Vector2< Type > floor( Vector2< Type > const & vector2D );
    template < C_Primitive Type >
    Vector2< Type > round( Vector2< Type > const & vector2D );

    template < C_Primitive Type >
    std::ostream & operator<<( std::ostream & stream,
                               Vector2< Type > const & vector2D );

    /* ************************************************************************
    ************************** VECTOR VECTOR ********************************
    ************************************************************************ */

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator*( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight );
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator/( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight );
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator+( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight );
    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator-( Vector2< TypeLeft > const & vector2DLeft,
                                   Vector2< TypeRight > const & vector2DRight );

    /* ************************************************************************
    ************************** VECTOR * FACTOR ********************************
    ************************************************************************ */

    template < C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator*( Vector2< VectorType > const & vector2D,
                                     FactorType const & factor );
    template < C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator/( Vector2< VectorType > const & vector2D,
                                     FactorType const & factor );

    template < C_Primitive Type >
    Vector2< Type > operator%( Vector2< Type > const & vector2D,
                               int const & modulo );

    template < C_Primitive Type >
    Vector2< Type > operator%( Vector2< Type > const & vector2D,
                               unsigned int const & modulo );

    /* ************************************************************************
    **************************** SIGN OPERATOR ********************************
    ************************************************************************ */

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator==( Vector2< TypeLeft > const & vector2DLeft,
                     Vector2< TypeRight > const & vector2DRight );

    template < C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator<( Vector2< TypeLeft > const & vector2DLeft,
                    Vector2< TypeRight > const & vector2DRight );

    /* ************************************************************************
    ************************** UNARY VECTOR ********************************
    ************************************************************************ */

    template < C_RelativePrimitive Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2D );

    template < C_RelativePrimitive Type >
    Vector2< Type > operator-=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight );

    using Vector2F = Vector2< float >;
    using Vector2I = Vector2< int >;
    using Vector2U = Vector2< unsigned int >;
} // namespace math

#include "vector2.tpp"