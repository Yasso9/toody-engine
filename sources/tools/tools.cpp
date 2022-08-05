#include "tools.hpp"

#include <cmath>     // for trunc
#include <fstream>   // for basic_ostream, basic_ostream::operator<<, ifstream
#include <iostream>  // for cerr
#include <sstream>   // for stringstream
#include <stdint.h>  // for intptr_t, uintptr_t

namespace tools
{
    bool is_integer ( float const & number )
    {
        return std::trunc( number ) != number;
    }

    std::string read_file ( std::string const & fileName )
    {
        std::ifstream file { fileName, std::ios::in };
        if ( ! file )
        {
            std::cerr << "File not found" << fileName << std::endl;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void * cast_int_to_void_pointer ( int const & integer )
    {
        return reinterpret_cast< void * >( static_cast< intptr_t >( integer ) );
    }

    void * cast_unsigned_int_to_void_pointer ( int const & unsignedInteger )
    {
        return reinterpret_cast< void * >(
            static_cast< uintptr_t >( unsignedInteger ) );
    }

}  // namespace tools