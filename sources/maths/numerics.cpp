#include "numerics.hpp"

#include <cmath>

namespace math
{
    unsigned int division_reminder( float const & leftValue,
                                    float const & rightValue )
    {
        return static_cast< unsigned int >(
            std::fmod( leftValue, rightValue ) );
    }
} // namespace math
