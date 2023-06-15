#include "rect.hpp"

namespace math
{
    // ####################################################################
    // ######################### RECT op RECT #############################
    // ####################################################################

    template< cPrimitive T >
    Rectangle< T > operator+ ( Rectangle< T > const & lhs,
                               Rectangle< T > const & rhs )
    {
        return Rectangle< T > { lhs.pos + rhs.pos, lhs.size + rhs.size };
    }

    template< cPrimitive T >
    Rectangle< T > operator- ( Rectangle< T > const & lhs,
                               Rectangle< T > const & rhs )
    {
        return Rectangle< T > { lhs.pos - rhs.pos, lhs.size - rhs.size };
    }

    template< cPrimitive T >
    Rectangle< T > operator+= ( Rectangle< T > &       lhs,
                                Rectangle< T > const & rhs )
    {
        return lhs = lhs + rhs;
    }

    template< cPrimitive T >
    Rectangle< T > operator-= ( Rectangle< T > &       lhs,
                                Rectangle< T > const & rhs )
    {
        return lhs = lhs - rhs;
    }

    // ####################################################################
    // ########################## RECT op VEC #############################
    // ####################################################################

    template< cPrimitive T >
    Rectangle< T > operator+ ( Rectangle< T > const & rect,
                               Vector2< T > const &   vec2 )
    {
        return Rectangle< T > { rect.pos + vec2, rect.size };
    }

    template< cPrimitive T >
    Rectangle< T > operator- ( Rectangle< T > const & rect,
                               Vector2< T > const &   vec2 )
    {
        return Rectangle< T > { rect.pos - vec2, rect.size };
    }

    template< cPrimitive T >
    Rectangle< T > operator+= ( Rectangle< T > &     rect,
                                Vector2< T > const & vec2 )
    {
        return rect = rect + vec2;
    }

    template< cPrimitive T >
    Rectangle< T > operator-= ( Rectangle< T > &     rect,
                                Vector2< T > const & vec2 )
    {
        return rect = rect - vec2;
    }
}  // namespace math
