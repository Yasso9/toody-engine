#pragma once

#include <ostream>
#include <type_traits>
#include <vector>

/// @brief Require numbers type like float, int or unsigned int
template< typename Type >
concept C_Primitive =
    std::is_integral_v< Type > || std::is_floating_point_v< Type >;

/// @brief Require numbers type like float, int but not unsigned int
template< typename Type >
concept C_RelativePrimitive = C_Primitive< Type > && not
std::is_unsigned_v< Type >;

namespace math
{
    template< C_Primitive Type >
    class Point;
}  // namespace math

template< typename Type >
concept C_PrintableObject = requires ( Type type, std::ostream ostream ) {
                                {
                                    type.operator<< ( ostream )
                                    } -> std::same_as< std::ostream & >;
                            };

template< typename Type >
concept C_Printable =
    requires ( Type type, std::ostream stream ) { stream << type; };

/// @brief Number greater than zero
template< unsigned int number > concept C_Size = ( number >= 1u );