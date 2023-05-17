#pragma once

#include "tools.hpp"

namespace tools
{
    template< typename Type >
    std::size_t size_of ( std::vector< Type > const & array )
    {
        return array.size() * sizeof( Type );
    }

    template< typename Type >
    Type const * to_c_style_array ( std::vector< Type > const & array )
    {
        return &array[0];
    }
}  // namespace tools
