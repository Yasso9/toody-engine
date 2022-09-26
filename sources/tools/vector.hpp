#pragma once

#include <vector>

namespace vector
{
    template< typename Type >
    std::vector< Type > extract (
        std::vector< Type > vector, unsigned int begin, unsigned int end );

    template< typename Type >
    void append (
        std::vector< Type > & vectorA, std::vector< Type > const & vectorB );

    /// @brief Check if the all the size of the subvectors are equal
    template< typename Type >
    bool is_rectangle (
        std::vector< std::vector< Type > > const & dimensionnalVector );
}  // namespace vector

#include "vector.tpp"