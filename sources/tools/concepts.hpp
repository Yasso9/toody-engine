#pragma once

#include <type_traits>

/// @brief Require numbers type like float, int or unsigned int
template < typename Type >
concept C_Primitive =
    std::is_integral_v< Type > || std::is_floating_point_v< Type >;

/// @brief Require numbers type like float, int but not unsigned int
template < typename Type >
concept C_RelativePrimitive = C_Primitive< Type > || std::is_unsigned_v< Type >;

template < typename Type >
concept C_Printable = requires( Type type, std::ostream ostream )
{
    {
        type.operator<<( ostream )
        } -> std::same_as< std::ostream & >;
};

/// @brief Number greater than zero
template < unsigned int number >
concept C_Size = ( number >= 1u );