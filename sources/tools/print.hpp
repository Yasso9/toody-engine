#pragma once

#include <sstream>

#include "tools/concepts.hpp"

template< C_Printable Type >
std::ostream & operator<< ( std::ostream & stream, Type const & tileData )
{
    tileData.operator<< ( stream );
    return stream;
}