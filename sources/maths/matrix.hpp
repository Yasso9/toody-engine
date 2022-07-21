#pragma once

#include <array>
#include <vector>

#include "vector2.hpp"

namespace math
{
    class Matrix2
    {
        std::array< Vector2< float >, 2 > m_vectorArray;

      public:
        Matrix2( Vector2< float > const & vectorLine1,
                 Vector2< float > const & vectorLine2 );
        virtual ~Matrix2() = default;

        Vector2< float > operator[]( std::size_t index ) const;
        float get_determining_factor() const;
    };
} // namespace math