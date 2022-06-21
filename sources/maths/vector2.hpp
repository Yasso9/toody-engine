#pragma once

#include <iostream>
#include <utility>

#include "graphics2D/sfml.hpp"
#include "libraries/imgui.hpp"
#include "tools/concepts.hpp"

namespace math
{
    // Vector2 is used in Rectangle, we need to forward declare
    template < C_Primitive Type >
    class Rectangle;

    template < C_Primitive Type >
    class Vector2;
    using Vector2F = Vector2< float >;
    using Vector2I = Vector2< int >;
    using Vector2U = Vector2< unsigned int >;

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
        constexpr virtual ~Vector2() = default;

        /// @brief copy constructor
        Vector2( Vector2< Type > const & vector2D );
        /// @brief move constructor
        Vector2( Vector2< Type > && vector2D ) noexcept;
        /// @brief copy assignement
        Vector2< Type > & operator=( Vector2< Type > const & vector2D );
        /// @brief move assignement
        Vector2< Type > & operator=( Vector2< Type > && vector2D ) noexcept;

        operator sf::Vector2< Type >() const;
        /// @brief conversion to ImVec2 must be possible only if the same type
        operator ImVec2() const
            requires( std::is_same_v< Type, decltype( ImVec2::x ) > );
        template < C_Primitive OtherType >
            requires( not std::is_same_v< Type, OtherType > )
        explicit operator Vector2< OtherType >() const;

        Vector2< float > to_float() const
            requires( not std::is_same_v< Type, float > );
        Vector2< unsigned int > to_u_int() const
            requires( not std::is_same_v< Type, unsigned int > );
        Vector2< int > to_int() const
            requires( not std::is_same_v< Type, int > );
        Vector2< std::size_t > to_size_t() const
            requires( not std::is_same_v< Type, std::size_t > );

        virtual Type operator[]( std::size_t index ) const;

        bool is_inside( Rectangle< Type > const & rectangle ) const;
        bool is_inside( Vector2< Type > const & position,
                        Vector2< Type > const & size ) const;
        void floor();
        void round();
    };

    /* ************************************************************************
    ************************** VECTOR FUNCTIONS *******************************
    ************************************************************************ */

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
} // namespace math

#include "vector2.tpp"