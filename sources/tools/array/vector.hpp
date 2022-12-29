#pragma once

#include <vector>

namespace vector
{
    /// @brief Get the subvector between the two index of vector
    /// @tparam Type
    /// @param vector
    /// @param begin
    /// @param end
    /// @return
    template< typename Type >
    std::vector< Type > extract ( std::vector< Type > vector,
                                  unsigned int begin, unsigned int end );

    /// @brief Checking if a vector contain an element
    /// @tparam Type Type of the elements of the vector
    /// @param vector Array in which we will search for our elements
    /// @param elementToSearch element that we will be searching
    /// @return true if the element is contained in the vector, false
    /// otherwise
    template< typename Type >
    bool contains ( std::vector< Type > const & vector,
                    Type const &                elementToSearch );

    /// @brief Add vectorB to vectorA
    template< typename Type >
    void append ( std::vector< Type > &       vectorA,
                  std::vector< Type > const & vectorB );

    /// @brief Check if the all the size of the subvectors are equal
    template< typename Type >
    bool is_rectangle (
        std::vector< std::vector< Type > > const & dimensionnalVector );
}  // namespace vector

#include "vector.tpp"