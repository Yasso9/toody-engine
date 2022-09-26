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
}  // namespace vector

#include "vector.tpp"