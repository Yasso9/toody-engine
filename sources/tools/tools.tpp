#pragma once

#include "tools.hpp"

namespace tools
{
    template < typename Type >
    std::size_t size_of( std::vector< Type > const & array )
    {
        return array.size() * sizeof( Type );
    }

    template < typename Type >
    Type const * to_c_style_array( std::vector< Type > const & array )
    {
        return &array[0];
    }

    template < typename Type >
    bool is_rectangle(
        std::vector< std::vector< Type > > const & dimensionnalVector )
    {
        if ( dimensionnalVector.empty() )
        {
            // An empty table is considered like a recangle table
            return true;
        }

        bool sameSizeForAllColumn { true };
        std::size_t baseColumnSize { dimensionnalVector[0].size() };
        for ( auto const & element : dimensionnalVector )
        {
            if ( baseColumnSize != element.size() )
            {
                sameSizeForAllColumn = false;
                break;
            }
        }

        return sameSizeForAllColumn;
    }
} // namespace tools
