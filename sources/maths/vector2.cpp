#include "vector2.hpp"

#include "tools/assertion.hpp"

#include "rectangle.hpp"

namespace math
{
    template < typename Type >
    Vector2< Type >::Vector2( sf::Vector2< Type > const & sfmlVector )
      : x( sfmlVector.x ), y( sfmlVector.y )
    {}
    template < typename Type >
    Vector2< Type >::Vector2( ImVec2 const & imGuiVector )
      : x( imGuiVector.x ), y( imGuiVector.y )
    {}

    template < typename Type >
    Vector2< Type >::operator sf::Vector2< Type >() const
    {
        return sf::Vector2< Type > { this->x, this->y };
    }
    template < typename Type >
    Vector2< Type >::operator ImVec2() const
    {
        return ImVec2 { this->x, this->y };
    }

    template < typename Type >
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

    template < typename Type >
    bool Vector2< Type >::is_inside( Rectangle const & rectangle ) const
    {
        return math::is_inside( *this, rectangle.position, rectangle.size );
    }
    template < typename Type >
    bool Vector2< Type >::is_inside( Vector2< Type > const & position,
                                     Vector2< Type > const & size ) const
    {
        return math::is_inside( *this, position, size );
    }
    template < typename Type >
    void Vector2< Type >::floor()
    {
        this->x = std::floor( this->x );
        this->y = std::floor( this->y );
    }
    template < typename Type >
    void Vector2< Type >::round()
    {
        this->x = std::round( this->x );
        this->y = std::round( this->y );
    }

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

    template < typename Type >
    bool is_inside( Vector2< Type > const & value,
                    Vector2< Type > const & position,
                    Vector2< Type > const & size )
    {
        return ( value.x >= position.x && value.x < position.x + size.x
                 && value.y >= position.y && value.y < position.y + size.y );
    }

    // template class Vector2< float >;
    // template class Vector2< int >;
    // template class Vector2< unsigned int >;
} // namespace math