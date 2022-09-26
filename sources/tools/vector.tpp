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
}  // namespace vector