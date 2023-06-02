#pragma once

#include <ostream>
#include <type_traits>
#include <vector>

/// @brief Require numbers type like float, int or unsigned int
template< typename Type >
concept cPrimitive = std::is_integral_v< Type >
                     || std::is_floating_point_v< Type >;

/// @brief Require numbers type like float, int but not unsigned int
template< typename Type >
concept cRelativePrimitive = cPrimitive< Type >
                             && not std::is_unsigned_v< Type >;

namespace math
{
    template< cPrimitive Type >
    class Point;
}  // namespace math

template< typename Type >
concept cPrintableObject = requires ( Type type, std::ostream ostream ) {
                               {
                                   type.operator<< ( ostream )
                               } -> std::same_as< std::ostream & >;
                           };

template< typename Type >
concept cPrintable = requires ( Type type, std::ostream stream ) {
                         stream << type;
                     };

/// @brief Number greater than zero
template< unsigned int number > concept cSize = ( number >= 1u );
