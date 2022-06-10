#pragma once

#include <iostream>

#include "graphics2D/sfml.hpp"
#include "tools/imgui.hpp"

namespace math
{
    class Rectangle;

    template < typename Type >
    class Vector2
    {
      public:
        Type x;
        Type y;

        constexpr Vector2() : x( 0 ), y( 0 ) {}
        // constexpr Vector2( Type const & xAxisValue, Type const & yAxisValue )
        //   : x( xAxisValue ), y( yAxisValue )
        // {}
        constexpr Vector2( float const & xAxisValue, float const & yAxisValue )
          : x( static_cast< Type >( xAxisValue ) ),
            y( static_cast< Type >( yAxisValue ) )
        {}
        constexpr Vector2( unsigned int const & xAxisValue,
                           unsigned int const & yAxisValue )
          : x( static_cast< Type >( xAxisValue ) ),
            y( static_cast< Type >( yAxisValue ) )
        {}
        constexpr Vector2( int const & xAxisValue, int const & yAxisValue )
          : x( static_cast< Type >( xAxisValue ) ),
            y( static_cast< Type >( yAxisValue ) )
        {}
        // constexpr Vector2( Vector2< float > const & vector )
        //   : x( static_cast< Type >( vector.x ) ),
        //     y( static_cast< Type >( vector.y ) )
        // {}
        // constexpr Vector2( Vector2< unsigned int > const & vector )
        //   : x( static_cast< Type >( vector.x ) ),
        //     y( static_cast< Type >( vector.y ) )
        // {}
        // constexpr Vector2( Vector2< int > const & vector )
        //   : x( static_cast< Type >( vector.x ) ),
        //     y( static_cast< Type >( vector.y ) )
        // {}
        Vector2( sf::Vector2< Type > const & sfmlVector );
        Vector2( ImVec2 const & imGuiVector );
        constexpr virtual ~Vector2() {};

        operator sf::Vector2< Type >() const;
        operator ImVec2() const;

        virtual Type operator[]( std::size_t index ) const;

        bool is_inside( Rectangle const & rectangle ) const;
        bool is_inside( Vector2< Type > const & position,
                        Vector2< Type > const & size ) const;
        void floor();
        void round();
    };

    template < typename Type >
    std::ostream & operator<<( std::ostream & stream,
                               Vector2< Type > const & vector2D );

    template < typename Type >
    Vector2< Type > operator*( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight );
    template < typename Type >
    Vector2< Type > operator/( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight );
    template < typename Type >
    Vector2< Type > operator+( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight );
    template < typename Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight );

    template < typename Type >
    Vector2< Type > operator*( Vector2< Type > const & vector2D,
                               float const & factor );
    template < typename Type >
    Vector2< Type > operator/( Vector2< Type > const & vector2D,
                               float const & factor );

    template < typename Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2D );

    template < typename Type >
    Vector2< Type > operator-=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight );

    template < typename Type >
    bool is_inside( Vector2< Type > const & value,
                    Vector2< Type > const & position,
                    Vector2< Type > const & size );
} // namespace math

#include "vector2.cpp"