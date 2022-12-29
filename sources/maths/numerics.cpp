#include "numerics.hpp"

#include <cmath>    // for fmod
#include <sstream>  // for stringstream, basic_ostream

#include <bits/std_abs.h>  // for abs

#include "maths/vector2.hpp"

namespace math
{
    bool are_equal ( float leftValue, float rightValue, float precision )
    {
        return std::abs( leftValue - rightValue ) < precision;
    }

    float whole_part ( float number )
    {
        return std::floor( number );
    }

    math::Vector2F whole_part ( math::Vector2F vector )
    {
        return math::Vector2F { whole_part( vector.x ),
                                whole_part( vector.y ) };
    }

    bool is_whole_number ( float number )
    {
        return std::floor( number ) == number;
    }

    std::string get_full_value ( float value )
    {
        std::stringstream stream {};

        stream.precision( std::numeric_limits< float >::max_digits10 );
        stream << value;

        return stream.str();
    }

    float division_reminder ( float leftValue, float rightValue )
    {
        return std::fmod( leftValue, rightValue );
    }

    unsigned int division_reminder_u ( float leftValue, float rightValue )
    {
        return static_cast< unsigned int >(
            division_reminder( leftValue, rightValue ) );
    }
}  // namespace math
