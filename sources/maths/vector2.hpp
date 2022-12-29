#pragma once

#include <iostream>
#include <utility>

#include <IMGUI/imgui.h>

#include "graphics2D/sfml.hpp"
#include "tools/concepts.hpp"
#include "tools/stream/stream.hpp"

namespace math
{
    // Vector2 is used in Rectangle, we need to forward declare
    template< C_Primitive Type >
    class Rectangle;

    template< C_Primitive Type >
    class Point;

    template< C_Primitive Type >
    class Vector2;
    using Vector2F = Vector2< float >;
    using Vector2I = Vector2< int >;
    using Vector2U = Vector2< unsigned int >;

    template< C_Primitive Type >
    class Vector2
    {
      public:
        Type x;
        Type y;

        /* ********************************************************************
        **************************** CONSTRUCTORS *****************************
        ******************************************************************** */

        constexpr Vector2() noexcept : x( 0 ), y( 0 ) {}

        constexpr Vector2( Type xAxisValue, Type yAxisValue ) noexcept
          : x( xAxisValue ), y( yAxisValue )
        {}

        Vector2( sf::Vector2< Type > const & sfmlVector );
        Vector2( ImVec2 const & imGuiVector );
        constexpr virtual ~Vector2() = default;

        /* ********************************************************************
        ******************************** COPY *********************************
        ******************************************************************** */

        /// @brief Copy Constructor
        Vector2( Vector2< Type > const & vector2D ) noexcept;
        /// @brief Move Constructor
        Vector2( Vector2< Type > && vector2D ) noexcept;
        /// @brief Copy Assignement
        Vector2< Type > & operator= (
            Vector2< Type > const & vector2D ) noexcept;
        /// @brief Move Assignement
        Vector2< Type > & operator= ( Vector2< Type > && vector2D ) noexcept;
        /// @brief Array operator
        Type              operator[] ( std::size_t index ) const;

        /* ********************************************************************
         *************************** STREAM OPERATOR **************************
         ******************************************************************* */

        template< C_Primitive TypeStream >
        friend std::ostream & operator<< ( std::ostream &          stream,
                                           Vector2< Type > const & vector );
        template< C_Primitive TypeStream >
        friend std::istream & operator>> ( std::istream &    stream,
                                           Vector2< Type > & vector );

        /* ********************************************************************
        ************************ ASSIGNEMENT OPERATOR *************************
        ******************************************************************** */

        Vector2< Type > operator-= ( Vector2< Type > rhs );
        Vector2< Type > operator+= ( Vector2< Type > rhs );
        Vector2< Type > operator*= ( Vector2< Type > rhs );
        Vector2< Type > operator/= ( Vector2< Type > rhs );
        Vector2< Type > operator/= ( float factor );

        /* ********************************************************************
        ******************************* CAST **********************************
        ******************************************************************** */

        operator sf::Vector2< Type > () const;
        /// @brief conversion to ImVec2 must be possible only if the same type
        operator ImVec2 () const
            requires ( std::is_same_v< Type, decltype( ImVec2::x ) > );
        template< C_Primitive OtherType >
        requires ( not std::is_same_v< Type, OtherType > )
        explicit operator Vector2< OtherType > () const;

        Vector2< float > to_float () const
            requires ( not std::is_same_v< Type, float > );
        Vector2< unsigned int > to_u_int () const
            requires ( not std::is_same_v< Type, unsigned int > );
        Vector2< int > to_int () const
            requires ( not std::is_same_v< Type, int > );
        Vector2< std::size_t > to_size_t () const
            requires ( not std::is_same_v< Type, std::size_t > );

        Point< Type > to_point () const;

        /* ********************************************************************
        **************************** METHODS **********************************
        ******************************************************************** */

        /// @brief Check if the vector's values represent a whole number
        /// @tparam Type type of the vector
        /// @return true if the vector's values represent a whole number, false
        /// otherwise
        bool is_whole () const;

        /// @brief get the biggest value between x and y
        Type get_max () const;
        /// @brief get the lowest value between x and y
        Type get_min () const;

        Vector2< Type > get_x_axis () const;
        Vector2< Type > get_y_axis () const;

        Vector2< Type > floor ();
        Vector2< Type > round ();

        float           get_length () const;
        Vector2< Type > get_norm () const;
        Vector2< Type > normalize ();
    };

    /* ************************************************************************
    ************************** VECTOR FUNCTIONS *******************************
    ************************************************************************ */

    template< C_Primitive Type >
    Vector2< Type > floor ( Vector2< Type > const & vector2D );
    template< C_Primitive Type >
    Vector2< Type > round ( Vector2< Type > const & vector2D );

    /* ************************************************************************
    ************************** VECTOR VECTOR ********************************
    ************************************************************************ */

    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator* (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator/ (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator+ (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    Vector2< TypeLeft > operator- (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );

    /* ************************************************************************
    ************************** VECTOR * FACTOR ********************************
    ************************************************************************ */

    template< C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator* ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor );
    template< C_Primitive VectorType, C_Primitive FactorType >
    Vector2< VectorType > operator/ ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor );

    template< C_Primitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                int const &             modulo );
    template< C_Primitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                unsigned int const &    modulo );

    /* ************************************************************************
    ****************************** COMPARISON *********************************
    ************************************************************************ */

    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator== ( Vector2< TypeLeft > const &  vector2DLeft,
                      Vector2< TypeRight > const & vector2DRight );

    template< C_Primitive TypeLeft, C_Primitive TypeRight >
    bool operator<( Vector2< TypeLeft > const &  vector2DLeft,
                    Vector2< TypeRight > const & vector2DRight );

    template< C_Primitive Type >
    bool operator> ( Vector2< Type > const & vector2DLeft,
                     Vector2< Type > const & vector2DRight );

    /* ************************************************************************
    ********************************** UNARY **********************************
    ************************************************************************ */

    template< C_RelativePrimitive Type >
    Vector2< Type > operator- ( Vector2< Type > const & vector2D );
}  // namespace math

#include "vector2.tpp"