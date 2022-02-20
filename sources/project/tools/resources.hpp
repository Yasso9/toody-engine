#pragma once

#include <vector>

#include "tools/string.hpp"

namespace too
{
    template <typename Type>
    std::size_t size_of( std::vector<Type> const & array );

    template <typename Type>
    Type * to_c_style_array( std::vector<Type> & array );

    std::string get_resources_path();
    std::string get_shaders_path();
} // namespace too

#include "resources.tpp"