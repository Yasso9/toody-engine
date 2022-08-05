#pragma once

#include <limits>  // for numeric_limits
#include <string>  // for string

namespace math
{
    /// @brief comparion of float wit tolerance
    bool are_equal (
        float leftValue, float rightValue,
        float precision = std::numeric_limits< float >::epsilon() );

    /// @brief get complete float value with maximum precision
    std::string get_full_value ( float value );

    float        division_reminder ( float leftValue, float rightValue );
    unsigned int division_reminder_u ( float leftValue, float rightValue );
}  // namespace math