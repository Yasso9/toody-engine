#pragma once

#include <iostream>
#include <utility>

#include <fmt/format.h>
#include <imgui/imgui.h>

#include "graphics2D/sfml.hpp"
#include "tools/concepts.hpp"
#include "tools/stream/stream.hpp"

namespace math
{
    // Vector2 is used in Rectangle, we need to forward declare
    template< cPrimitive Type >
    class Rectangle;

    template< cPrimitive Type >
    class Point;

    template< cPrimitive Type >
    class Vector2;
    using Vector2F = Vector2< float >;
    using Vector2I = Vector2< int >;
    using Vector2U = Vector2< unsigned int >;

    template< cPrimitive Type >
    class Vector2
    {
      public:
        Type x;
        Type y;

        // ####################################################################
        // ########################## CONSTRUCTORS ############################
        // ####################################################################

        constexpr Vector2() noexcept : x { 0 }, y { 0 } {}

        constexpr Vector2( Type xAxisValue, Type yAxisValue ) noexcept
          : x { xAxisValue }, y { yAxisValue }
        {}

        Vector2( sf::Vector2< Type > const & sfmlVector );
        Vector2( ImVec2 const & imGuiVector );
        constexpr virtual ~Vector2() = default;

        // ####################################################################
        // ########################## RULE OF FIVE ############################
        // ####################################################################

        /// @brief Copy Constructor
        Vector2( Vector2< Type > const & vector2D ) noexcept;
        /// @brief Move Constructor
        Vector2( Vector2< Type > && vector2D ) noexcept;
        /// @brief Copy Assignement
        Vector2< Type > & operator= (
            Vector2< Type > const & vector2D ) noexcept;
        /// @brief Move Assignement
        Vector2< Type > & operator= ( Vector2< Type > && vector2D ) noexcept;

        // ####################################################################
        // ######################### STATIC MEMBERS ###########################
        // ####################################################################

        constexpr static Vector2< Type > const ZERO { 0, 0 };

        // ####################################################################
        // ######################## STREAM OPERATORS ##########################
        // ####################################################################

        template< cPrimitive TypeStream >
        friend std::ostream & operator<< ( std::ostream &          stream,
                                           Vector2< Type > const & vector );
        template< cPrimitive TypeStream >
        friend std::istream & operator>> ( std::istream &    stream,
                                           Vector2< Type > & vector );

        // ####################################################################
        // ##################### ASSIGNEMENT OPERATORS ########################
        // ####################################################################

        Vector2< Type > operator-= ( Vector2< Type > rhs );
        Vector2< Type > operator+= ( Vector2< Type > rhs );
        Vector2< Type > operator*= ( Vector2< Type > rhs );
        Vector2< Type > operator/= ( Vector2< Type > rhs );
        Vector2< Type > operator/= ( float factor );

        // ####################################################################
        // ######################## OTHER OPERATORS ###########################
        // ####################################################################

        /// @brief Array operator
        Type operator[] ( std::size_t index ) const;

        // ####################################################################
        // ############################### CAST ###############################
        // ####################################################################

        operator sf::Vector2< Type > () const;
        /// @brief conversion to ImVec2 must be possible only if the same type
        operator ImVec2 () const
            requires ( std::is_same_v< Type, decltype( ImVec2::x ) > );
        template< cPrimitive OtherType >
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

        // ####################################################################
        // ############################## METHODS #############################
        // ####################################################################

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

        template< cPrimitive T >
        friend Vector2< T > max ( Vector2< T > const & lhs,
                                  Vector2< T > const & rhs );
        template< cPrimitive T >
        friend Vector2< T > min ( Vector2< T > const & lhs,
                                  Vector2< T > const & rhs );
    };

    // ####################################################################
    // ######################### VECTOR FUNCTIONS #########################
    // ####################################################################

    template< cPrimitive Type >
    Vector2< Type > floor ( Vector2< Type > const & vector2D );
    template< cPrimitive Type >
    Vector2< Type > round ( Vector2< Type > const & vector2D );

    // ####################################################################
    // ########################### VECTOR VECTOR ##########################
    // ####################################################################

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator* (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator/ (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator+ (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );
    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    Vector2< TypeLeft > operator- (
        Vector2< TypeLeft > const &  vector2DLeft,
        Vector2< TypeRight > const & vector2DRight );

    // ####################################################################
    // ########################## VECTOR * FACTOR #########################
    // ####################################################################

    template< cPrimitive VectorType, cPrimitive FactorType >
    Vector2< VectorType > operator* ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor );
    template< cPrimitive VectorType, cPrimitive FactorType >
    Vector2< VectorType > operator/ ( Vector2< VectorType > const & vector2D,
                                      FactorType const &            factor );

    template< cPrimitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                int const &             modulo );
    template< cPrimitive Type >
    Vector2< Type > operator% ( Vector2< Type > const & vector2D,
                                unsigned int const &    modulo );

    // ####################################################################
    // ########################### COMPARISON #############################
    // ####################################################################

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    bool operator== ( Vector2< TypeLeft > const &  vector2DLeft,
                      Vector2< TypeRight > const & vector2DRight );

    template< cPrimitive TypeLeft, cPrimitive TypeRight >
    bool operator<( Vector2< TypeLeft > const &  vector2DLeft,
                    Vector2< TypeRight > const & vector2DRight );

    template< cPrimitive Type >
    bool operator> ( Vector2< Type > const & vector2DLeft,
                     Vector2< Type > const & vector2DRight );

    // ####################################################################
    // ############################## UNARY ###############################
    // ####################################################################

    template< cRelativePrimitive Type >
    Vector2< Type > operator- ( Vector2< Type > const & vector2D );
}  // namespace math

template< cPrimitive T >
struct fmt::formatter< math::Vector2< T > >
{
    constexpr auto parse ( format_parse_context & ctx ) { return ctx.begin(); }

    template< typename FormatContext >
    auto format ( math::Vector2< T > const & vec2, FormatContext & ctx )
    {
        // The format for MyType is "{a:
        // <value_of_a>, b: <value_of_b>, c:
        // <value_of_c>}"
        return format_to( ctx.out(), "{{x: {}, y: {}}}", vec2.x, vec2.y );
    }
};

#include "vector2.tpp"
