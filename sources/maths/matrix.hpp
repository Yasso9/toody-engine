#pragma once

#include "vector2.hpp"

namespace math
{
    class Matrix2
    {
      public:
        Matrix2( Vector2< float > const & vectorLine1,
                 Vector2< float > const & vectorLine2 )
          : m_vectorArray( { vectorLine1, vectorLine2 } )
        {}
        virtual ~Matrix2() = default;

        Vector2< float > operator[]( std::size_t index ) const
        {
            return m_vectorArray[index];
        }
        float get_determining_factor() const
        {
            return ( this->m_vectorArray[0][0] * this->m_vectorArray[1][1] )
                   - ( this->m_vectorArray[0][1] * this->m_vectorArray[1][0] );
        }

      private:
        std::array< Vector2< float >, 2 > m_vectorArray;
    };
} // namespace math