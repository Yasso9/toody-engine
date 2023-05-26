#pragma once

#include <istream>
#include <ostream>
#include <type_traits>

template< typename T > concept cEnum = std::is_enum_v< T >;

template< cEnum E >
std::istream & operator>> ( std::istream & in, E & value );
template< cEnum E >
std::ostream & operator<< ( std::ostream & out, E const & value );

#include "enum_impl.hpp"
