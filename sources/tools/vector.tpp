#pragma once

#include "vector.hpp"

namespace vector
{
    template< typename Type >
    std::vector< Type > extract (
        std::vector< Type > vector, unsigned int begin, unsigned int end )
    {
        if ( begin >= vector.size() || end >= vector.size() )
        {
            std::ostringstream exceptionStream {};
            exceptionStream << "Range [ " << begin << ", " << end << " ]"
                            << " not comptatible with vector of size : '"
                            << vector.size() << "'";
            throw std::out_of_range { exceptionStream.str() };
        }

        return std::vector< Type > {
            vector.begin() + begin, vector.begin() + end };
    }

    template< typename Type >
    void append ( std::vector< Type > & a, std::vector< Type > const & b )
    {
        a.insert( a.end(), b.begin(), b.end() );
    }

    /// @brief Check if the all the size of the subvectors are equal
    template< typename Type >
    bool is_rectangle (
        std::vector< std::vector< Type > > const & dimensionnalVector )
    {
        if ( dimensionnalVector.empty() )
        {
            // An empty table is considered like a rectangle table
            return true;
        }

        bool        sameSizeForAllColumn { true };
        std::size_t baseColumnSize { dimensionnalVector[0].size() };
        for ( std::size_t i_subVector = 1;
              i_subVector < dimensionnalVector.size(); ++i_subVector )
        {
            if ( baseColumnSize != dimensionnalVector[i_subVector].size() )
            {
                sameSizeForAllColumn = false;
                break;
            }
        }

        return sameSizeForAllColumn;
    }
}  // namespace vector