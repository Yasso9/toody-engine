#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// @todo differences between typename or class in template ?
/// @todo Type require that the type have operator <<
template < typename Type >
std::ostream & operator<<( std::ostream & stream,
                           std::vector< Type > const & array );

template < typename Type >
std::istream & operator>>( std::istream & stream, std::vector< Type > & array );

bool verify_next( std::istream & stream, char const & character );

#include "serialization.hpp"