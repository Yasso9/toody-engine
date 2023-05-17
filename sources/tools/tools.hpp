#pragma once

#include <cstddef>  // for size_t
#include <string>   // for string
#include <vector>   // for vector

namespace tools
{
    bool is_integer ( float const & number );

    std::string read_file ( std::string const & fileName );

    void * cast_int_to_void_pointer ( int const & integer );
    void * cast_unsigned_int_to_void_pointer ( int const & unsignedInteger );

    template< typename Type >
    std::size_t size_of ( std::vector< Type > const & array );

    template< typename Type >
    Type const * to_c_style_array ( std::vector< Type > const & array );
}  // namespace tools
