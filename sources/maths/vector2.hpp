#pragma once

#include <iostream>

namespace math
{
    template < typename Type >
    class Vector2
    {
        Type x { 0 };
        Type y { 0 };

        constexpr Vector2() = default;
        constexpr Vector2( Type const & xAxisValue, Type const & yAxisValue )
          : x( xAxisValue ), y( yAxisValue )
        {}
        Vector2( sf::Vector2< Type > const & sfmlVector )
          : x( sfmlVector.x ), y( sfmlVector.y )
        {}
        // Vector2( ImVec2 const & imGuiVector )
        //   : Vector2( static_cast< sf::Vector2f >( imGuiVector ) )
        // {}
        constexpr virtual ~Vector2() {};

        virtual Type operator[]( std::size_t index ) const
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

        operator sf::Vector2< Type >() const
        {
            return sf::Vector2< Type > { this->x, this->y };
        }
        operator ImVec2() const { return ImVec2 { this->x, this->y }; }

        bool is_inside( Rectangle const & rectangle ) const
        {
            return math::is_inside( *this, rectangle.position, rectangle.size );
        }
        bool is_inside( Vector2< Type > const & position,
                        Vector2< Type > const & size ) const
        {
            return math::is_inside( *this, position, size );
        }

        void floor()
        {
            this->x = std::floor( this->x );
            this->y = std::floor( this->y );
        }
        void round()
        {
            this->x = std::round( this->x );
            this->y = std::round( this->y );
        }
    };

    template < typename Type >
    std::ostream & operator<<( std::ostream & stream,
                               Vector2< Type > const & vector2D )
    {
        return stream << "( " << vector2D.x << ", " << vector2D.y << " )";
    }

    template < typename Type >
    Vector2< Type > operator*( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight )
    {
        return { vector2DLeft.x * vector2DRight.x,
                 vector2DLeft.y * vector2DRight.y };
    }
    template < typename Type >
    Vector2< Type > operator/( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight )
    {
        return { vector2DLeft.x / vector2DRight.x,
                 vector2DLeft.y / vector2DRight.y };
    }
    template < typename Type >
    Vector2< Type > operator+( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight )
    {
        return { vector2DLeft.x + vector2DRight.x,
                 vector2DLeft.y + vector2DRight.y };
    }
    template < typename Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2DLeft,
                               Vector2< Type > const & vector2DRight )
    {
        return vector2DLeft + ( -vector2DRight );
    }

    template < typename Type >
    Vector2< Type > operator*( Vector2< Type > const & vector2D,
                               float const & factor )
    {
        return vector2D * Vector2 { factor, factor };
    }
    template < typename Type >
    Vector2< Type > operator/( Vector2< Type > const & vector2D,
                               float const & factor )
    {
        return vector2D / Vector2 { factor, factor };
    }

    template < typename Type >
    Vector2< Type > operator-( Vector2< Type > const & vector2D )
    {
        return vector2DLeft + ( -vector2DRight );
    }

    template < typename Type >
    Vector2< Type > operator-=( Vector2< Type > & vector2DLeft,
                                Vector2< Type > const & vector2DRight )
    {
        vector2DLeft = vector2DLeft - vector2DRight;
        return vector2DLeft;
    }
} // namespace math