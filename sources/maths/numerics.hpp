#pragma once

#include <limits>  // for numeric_limits
#include <string>  // for string

#include "tools/concepts.hpp"

namespace math
{
    template< cPrimitive Type >
    class Vector2;
}  // namespace math

namespace math
{
    /// @brief comparion of float wit tolerance
    bool are_equal ( float leftValue, float rightValue,
                     float precision =
                         std::numeric_limits< float >::epsilon() );

    /// @brief Check if the number is whole
    /// @param number the number to check
    /// @return true if the number is whole, false otherwise
    bool is_whole_number ( float number );

    /// @brief Get back the whole part of any number
    /// @param number
    /// @return Return the whole part of the number given
    float whole_part ( float number );

    /// @brief Get back the whole part of a vector
    /// @param vector
    /// @return Return the whole part of the vector given
    math::Vector2< float > whole_part ( math::Vector2< float > vector );

    /// @brief get complete float value with maximum precision
    std::string get_full_value ( float value );

    float        division_reminder ( float leftValue, float rightValue );
    unsigned int division_reminder_u ( float leftValue, float rightValue );
}  // namespace math
