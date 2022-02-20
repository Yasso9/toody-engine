#pragma once

#include "resources.hpp"

namespace too
{
    template <typename Type>
    std::size_t size_of( std::vector<Type> const & array )
    {
        return array.size() * sizeof( Type );
    }

    template <typename Type>
    Type * to_c_style_array( std::vector<Type> & array )
    {
        return &array[0];
    }
} // namespace too
