#include "matrix.hpp"

#include "maths/vector2.hpp"  // for Vector2
#include "maths/vector2.tpp"  // for Vector2::operator[], Vector2::Vector2<T...

namespace math
{
    Matrix2::Matrix2(
        Vector2< float > const & vectorLine1,
        Vector2< float > const & vectorLine2 )
      : m_vectorArray( { vectorLine1, vectorLine2 } )
    {}

    Vector2< float > Matrix2::operator[] ( std::size_t index ) const
    {
        return m_vectorArray[index];
    }

    float Matrix2::get_determining_factor() const
    {
        return ( this->m_vectorArray[0][0] * this->m_vectorArray[1][1] )
               - ( this->m_vectorArray[0][1] * this->m_vectorArray[1][0] );
    }
}  // namespace math